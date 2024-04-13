#include "videosave.h"
#include <QDebug>

extern "C"
{   // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

#define ERROR_LEN 1024   // 异常信息数组长度
#define PRINT_LOG 1
#define USE_H264 1   // 使用H264编码器

VideoSave::VideoSave() {}

VideoSave::~VideoSave()
{
    close();
}

/**
 * @brief        显示ffmpeg函数调用异常信息
 * @param err
 */
void VideoSave::showError(int err)
{
#if PRINT_LOG
    static char m_error[ERROR_LEN];   // 保存异常信息
    memset(m_error, 0, ERROR_LEN);    // 将数组置零
    av_strerror(err, m_error, ERROR_LEN);
    qWarning() << "VideoSave Error：" << m_error;
#else
    Q_UNUSED(err)
#endif
}

bool VideoSave::open(AVStream* inStream, const QString& fileName)
{
    if (!inStream || fileName.isEmpty())
        return false;

#if USE_H264
    // 通过输出文件名为输出格式分配AVFormatContext。(自动推测编码器)
    int ret = avformat_alloc_output_context2(&m_formatContext, nullptr, nullptr, fileName.toStdString().data());
#else
    /**
     * 摄像头打开使用的是mjpeg编码器；
     * MJPEG压缩技术可以获取清晰度很高的视频图像，可以【动态调整帧率】适合保存摄像头视频、分辨率。但由于没有考虑到帧间变化，造成大量冗余信息被重复存储，因此单帧视频的占用空间较大；
     * 如果采用其它编码器，由于摄像头曝光时间长度不一定，所以录像时帧率一直在变，编码器指定固定帧率会导致视频一会快一会慢，效果很不好，适用于录制固定帧率的视频（当然其它编码器应该是有处理办法，不过我还不清楚）；
     */
    QString strName = avcodec_find_encoder(inStream->codecpar->codec_id)->name;                                                         // 获取编码器名称
    int ret = avformat_alloc_output_context2(&m_formatContext, nullptr, strName.toStdString().data(), fileName.toStdString().data());   // 这里使用和解码一样的编码器，防止保存的图像颜色出问题
#endif
    if (ret < 0)
    {
        close();
        showError(ret);
        return false;
    }
    // 创建并初始化AVIOContext以访问url所指示的资源。
    ret = avio_open(&m_formatContext->pb, fileName.toStdString().data(), AVIO_FLAG_WRITE);
    if (ret < 0)
    {
        close();
        showError(ret);
        return false;
    }

    // 查询编码器
    const AVCodec* codec = avcodec_find_encoder(m_formatContext->oformat->video_codec);
    if (!codec)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }

    // 分配AVCodecContext并将其字段设置为默认值。
    m_codecContext = avcodec_alloc_context3(codec);
    if (!m_codecContext)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }
    // 设置编码器上下文参数
    m_codecContext->width = inStream->codecpar->width;   // 图片宽度/高度
    m_codecContext->height = inStream->codecpar->height;
#if USE_H264
    m_codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
#else
    m_codecContext->pix_fmt = AVPixelFormat(
        inStream->codecpar->format);   // 像素格式，也可以使用codec->pix_fmts[0]或AV_PIX_FMT_YUVJ422P(【注意】摄像头解码的图像格式为yuvj422p，如果这里不一样可能保存会出问题，或者后面进行格式转换)
#endif
    m_codecContext->time_base = {1, 30};   //设置时间基，30为分母，1为分子，表示以1/30秒时间间隔播放一帧图像
    m_codecContext->framerate = {30, 1};
    m_codecContext->bit_rate = 4000000;   // 目标的码率，即采样的码率；显然，采样码率越大，视频大小越大，画质越高
    m_codecContext->gop_size = 10;        // I帧间隔
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
    //    qDebug() << m_codecContext->pix_fmt;

    // 打开编码器
    ret = avcodec_open2(m_codecContext, nullptr, nullptr);
#if USE_H264
    ret = avcodec_open2(m_codecContext, codec, nullptr);   // 使用h264时第一次打不开，第二次可以打卡，不知道什么原因
#endif
    if (ret < 0)
    {
        close();
        showError(ret);
        return false;
    }

    // 向媒体文件添加新流
    m_videoStream = avformat_new_stream(m_formatContext, nullptr);
    if (!m_videoStream)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }

    //拷贝一些参数，给codecpar赋值
    ret = avcodec_parameters_from_context(m_videoStream->codecpar, m_codecContext);
    if (ret < 0)
    {
        close();
        showError(ret);
        return false;
    }

    // 写入文件头
    ret = avformat_write_header(m_formatContext, nullptr);
    if (ret < 0)
    {
        close();
        showError(ret);
        return false;
    }
    m_writeHeader = true;

    // 分配一个AVPacket
    m_packet = av_packet_alloc();
    if (!m_packet)
    {
        close();
        showError(AVERROR(ENOMEM));
        return false;
    }
    qDebug() << "开始录制视频！";
    return true;
}

/**
 * @brief        写入数据
 * @param frame
 */
void VideoSave::write(AVFrame* frame)
{
    QMutexLocker locker(&m_mutex);
    if (!m_packet)
    {
        return;
    }

    if (frame)
    {
        frame->pts = m_index;
        m_index++;
    }

    // 将图像传入编码器
    int ret = avcodec_send_frame(m_codecContext, frame);
    if (ret < 0)
    {
        showError(ret);
        return;
    }

    // 循环读取所有编码完的帧
    while (true)
    {
        // 从编码器中读取图像帧
        int ret = avcodec_receive_packet(m_codecContext, m_packet);
        if (ret < 0)
        {
            break;
        }

        // 将数据包中的有效时间字段（时间戳/持续时间）从一个时基转换为 输出流的时间
        av_packet_rescale_ts(m_packet, m_codecContext->time_base, m_videoStream->time_base);
        av_write_frame(m_formatContext, m_packet);   // 将数据包写入输出媒体文件
        av_packet_unref(m_packet);
    }
}

/**
 * @brief 关闭保存数据
 */
void VideoSave::close()
{
    write(nullptr);                  // 传入空帧，读取所有编码数据
    QMutexLocker locker(&m_mutex);   // 如果不加锁可能在点击关闭时，write函数正在写入数据，导致崩溃
    if (m_formatContext)
    {
        // 写入文件尾
        if (m_writeHeader)
        {
            m_writeHeader = false;
            int ret = av_write_trailer(m_formatContext);
            if (ret < 0)
            {
                showError(ret);
                return;
            }
        }
        int ret = avio_close(m_formatContext->pb);
        if (ret < 0)
        {
            showError(ret);
            return;
        }
        avformat_free_context(m_formatContext);
        m_formatContext = nullptr;
        m_videoStream = nullptr;
    }
    // 释放编解码器上下文并置空
    if (m_codecContext)
    {
        avcodec_free_context(&m_codecContext);
    }
    if (m_packet)
    {
        av_packet_free(&m_packet);
        qDebug() << "停止录制视频！";
    }

    m_index = 0;
}
