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
#define USE_H264 1      // 使用H264编码器

VideoCodec::VideoCodec()
{

}

VideoCodec::~VideoCodec()
{
    close();
}

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
#if USE_H264
    m_codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
#else
    m_codecContext->pix_fmt = AVPixelFormat(inStream->codecpar->format);        // 像素格式，也可以使用codec->pix_fmts[0]或AV_PIX_FMT_YUVJ422P(【注意】摄像头解码的图像格式为yuvj422p，如果这里不一样可能保存会出问题，或者后面进行格式转换)
#endif
    m_codecContext->time_base = {point.y(), point.x()};                   //设置时间基，20为分母，1为分子，表示以1/20秒时间间隔播放一帧图像
    m_codecContext->framerate = {point.x(), point.y()};
    m_codecContext->bit_rate = 4000000;                                   // 目标的码率，即采样的码率；显然，采样码率越大，视频大小越大，画质越高
    m_codecContext->gop_size = 12;                                        // I帧间隔
    m_codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//    m_codecContext->max_b_frames = 1;                      // 非B帧之间的最大B帧数(有些格式不支持)
//    m_codecContext->qmin = 1;
//    m_codecContext->qmax = 5;
//    m_codecContext->colorspace = AVCOL_SPC_BT470BG;
//    m_codecContext->color_range = AVCOL_RANGE_JPEG;
//    m_codecContext->color_primaries = AVCOL_PRI_BT709;
//    m_codecContext->bits_per_coded_sample = 24;
//    m_codecContext->bits_per_raw_sample = 8;
//    av_opt_set(m_codecContext->priv_data, "preset", "placebo", 0);

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
    qDebug() << "开始录制视频！";
    return true;
}

void VideoCodec::write(AVFrame *frame)
{
    QMutexLocker locker(&m_mutex);
    if(!m_packet)
    {
        return;
    }

    if(frame)
    {
        frame->pts = m_index;
        m_index++;
    }

    // 将图像传入编码器
    avcodec_send_frame(m_codecContext, frame);

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
    }
    // 释放编解码器上下文并置空
    if(m_codecContext)
    {
        avcodec_free_context(&m_codecContext);
    }
    if(m_packet)
    {
        av_packet_free(&m_packet);
        qDebug() << "停止录制视频！";
    }
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
