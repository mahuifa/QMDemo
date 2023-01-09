#include "videocodec.h"
#include <QDebug>

extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavdevice/avdevice.h"
}

#define ERROR_LEN 1024  // 异常信息数组长度
#define PRINT_LOG 1

VideoCodec::VideoCodec()
{

}

VideoCodec::~VideoCodec()
{
    close();
}

/**
 * @brief               初始化打开编码保存文件
 * @param codecContext
 * @param point
 * @param fileName
 * @return
 */
bool VideoCodec::open(AVCodecContext *codecContext, QPoint point, const QString &fileName)
{
    if(!codecContext || fileName.isEmpty()) return false;

    // 通过输出文件名为输出格式分配AVFormatContext。参数3编码器设置为空，由参数4文件名后缀推测合适的编码器
    int ret = avformat_alloc_output_context2(&m_formatContext, nullptr, nullptr, fileName.toStdString().data());

    if(ret < 0)
    {
        close();
        showError(ret);
        return false;
    }
    // 创建并初始化AVIOContext以访问url所指示的资源。
    ret = avio_open(&m_formatContext->pb, fileName.toStdString().data(), AVIO_FLAG_WRITE);
    if(ret < 0)
    {
        close();
        showError(ret);
        return false;
    }

    // 查询编码器
    const AVCodec* codec = avcodec_find_encoder(m_formatContext->oformat->video_codec);
    if(!codec)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }
    qDebug() << codec->id <<" " << codec->name;

    // 分配AVCodecContext并将其字段设置为默认值。
    m_codecContext = avcodec_alloc_context3(codec);
    if(!m_codecContext)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }

    // 设置编码器上下文参数
    m_codecContext->width = codecContext->width;                          // 图片宽度/高度
    m_codecContext->height = codecContext->height;
    m_codecContext->pix_fmt = codec->pix_fmts[0];                         // 像素格式（这里通过编码器赋值，不需要自己指定）
    m_codecContext->time_base = {point.y(), point.x()};                   //设置时间基，20为分母，1为分子，表示以1/20秒时间间隔播放一帧图像
    m_codecContext->framerate = {point.x(), point.y()};
    m_codecContext->bit_rate = 1000000;                                   // 目标的码率，即采样的码率；显然，采样码率越大，视频大小越大，画质越高
    m_codecContext->gop_size = 12;                                        // I帧间隔(值越大，视频文件越小，编解码延时越长)
    m_codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // 打开编码器
    ret = avcodec_open2(m_codecContext, nullptr, nullptr);
    if(ret < 0)
    {
        close();
        showError(ret);
        return false;
    }

    // 向媒体文件添加新流
    m_videoStream = avformat_new_stream(m_formatContext, nullptr);
    if(!m_videoStream)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }

    //拷贝一些参数，给codecpar赋值
    ret = avcodec_parameters_from_context(m_videoStream->codecpar,m_codecContext);
    if(ret < 0)
    {
        close();
        showError(ret);
        return false;
    }

    // 写入文件头
    ret = avformat_write_header(m_formatContext, nullptr);
    if(ret < 0)
    {
        close();
        showError(ret);
        return false;
    }
    m_writeHeader = true;

    // 分配一个AVPacket
    m_packet = av_packet_alloc();
    if(!m_packet)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }

    m_frame = av_frame_alloc();
    if(!m_frame)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }
    m_frame->format = codec->pix_fmts[0];

    qDebug() << "开始录制视频！";
    return true;
}

/**
 * @brief          将图像帧编码写入视频文件
 * @param frame
 */
void VideoCodec::write(AVFrame *frame)
{
    QMutexLocker locker(&m_mutex);
    if(!m_packet)
    {
        return;
    }

    if(!swsFormat(frame))              // 由于解码的图像格式和编码需要的图像格式不一定相同，所以需要转换一下格式
    {
        return;
    }
    if(m_frame)
    {
        m_frame->pts = m_index;         // pts从0开始增加，保存的视频才会时间从0开始增加
        m_index++;
    }


    avcodec_send_frame(m_codecContext, m_frame); // 将图像传入编码器

    // 循环读取所有编码完的帧
    while (true)
    {
        // 从编码器中读取图像帧
        int ret = avcodec_receive_packet(m_codecContext, m_packet);
        if(ret < 0)
        {
            break;
        }

        // 将数据包中的有效时间字段（时间戳/持续时间）从一个时基转换为 输出流的时间
        av_packet_rescale_ts(m_packet, m_codecContext->time_base, m_videoStream->time_base);
        av_write_frame(m_formatContext, m_packet);   // 将数据包写入输出媒体文件
        av_packet_unref(m_packet);
    }
}

void VideoCodec::close()
{
    write(nullptr);   // 传入空帧，读取所有编码数据
    QMutexLocker locker(&m_mutex);    // 如果不加锁可能在点击关闭时，write函数正在写入数据，导致崩溃
    if(m_formatContext)
    {
        // 写入文件尾
        if(m_writeHeader)
        {
            m_writeHeader = false;
            int ret = av_write_trailer(m_formatContext);
            if(ret < 0)
            {
                showError(ret);
                return;
            }
        }
        int ret = avio_close(m_formatContext->pb);
        if(ret < 0)
        {
            showError(ret);
            return;
        }
        avformat_free_context(m_formatContext);
        m_formatContext = nullptr;
        m_videoStream = nullptr;
    }
    // 释放编解码器上下文并置空
    if(m_codecContext)
    {
        avcodec_free_context(&m_codecContext);
    }
    if(m_packet)
    {
        av_packet_free(&m_packet);
    }
    // 释放上下文swsContext。
    if(m_swsContext)
    {
        sws_freeContext(m_swsContext);
        m_swsContext = nullptr;             // sws_freeContext不会把上下文置NULL
    }
    if(m_frame)
    {
        av_frame_free(&m_frame);
    }
    m_index = 0;
}

void VideoCodec::showError(int err)
{
#if PRINT_LOG
    static char  m_error[ERROR_LEN];         // 保存异常信息
    memset(m_error, 0, ERROR_LEN);           // 将数组置零
    av_strerror(err, m_error, ERROR_LEN);
    qWarning() << "VideoSave Error：" << m_error;
#else
    Q_UNUSED(err)
#endif
}

/**
 * @brief        将解码图像帧的像素格式转换未编码图像帧的像素格式
 * @param frame
 * @return       true：转换成功  false：转换失败
 */
bool VideoCodec::swsFormat(AVFrame *frame)
{
    if(!frame || frame->width <= 0 || frame->height <= 0)
    {
        return false;
    }
    // 为什么图像转换上下文要放在这里初始化呢，是因为m_frame->format，如果使用硬件解码，解码出来的图像格式和m_codecContext->pix_fmt的图像格式不一样，就会导致无法转换为QImage
    // 由于解码后的图像格式不一定支持保存裸流，或者不支持直接编码为H264，所以需要转换格式
    if(!m_swsContext)
    {
        // 获取缓存的图像转换上下文。首先校验参数是否一致，如果校验不通过就释放资源；然后判断上下文是否存在，如果存在直接复用，如不存在进行分配、初始化操作
        m_swsContext = sws_getCachedContext(m_swsContext,
                                            frame->width,                     // 输入图像的宽度
                                            frame->height,                    // 输入图像的高度
                                            (AVPixelFormat)frame->format,     // 输入图像的像素格式
                                            frame->width,                     // 输出图像的宽度
                                            frame->height,                    // 输出图像的高度
                                            (AVPixelFormat)m_frame->format,   // 输出图像的像素格式
                                            SWS_BILINEAR,                     // 选择缩放算法(只有当输入输出图像大小不同时有效),一般选择SWS_FAST_BILINEAR
                                            nullptr,                          // 输入图像的滤波器信息, 若不需要传NULL
                                            nullptr,                          // 输出图像的滤波器信息, 若不需要传NULL
                                            nullptr);                         // 特定缩放算法需要的参数(?)，默认为NULL
        if(!m_swsContext)
        {
#if PRINT_LOG
            qWarning() << "sws_getCachedContext() Error！";
#endif

            av_frame_unref(frame);
            return false;
        }

        if(m_frame)
        {
            // 创建一个图像帧用于保存YUV420P图像
            m_frame->width = frame->width;
            m_frame->height = frame->height;
            av_frame_get_buffer(m_frame, 3 * 8);
        }
    }

    if(m_frame->width <= 0 || m_frame->height <= 0)      // 如果m_frame没有分配空间则返回
    {
        return false;
    }

    // 开始转换格式
    bool ret = sws_scale(m_swsContext,             // 缩放上下文
                    frame->data,                   // 原图像数组
                    frame->linesize,               // 包含源图像每个平面步幅的数组
                    0,                             // 开始位置
                    frame->height,                 // 行数
                    m_frame->data,                 // 目标图像数组
                    m_frame->linesize);            // 包含目标图像每个平面的步幅的数组
    av_frame_unref(frame);
    return ret;
}
