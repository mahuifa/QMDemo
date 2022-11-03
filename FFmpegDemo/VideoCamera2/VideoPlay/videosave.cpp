#include "videosave.h"
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

VideoSave::VideoSave()
{
}

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
    static char  m_error[ERROR_LEN];         // 保存异常信息
    memset(m_error, 0, ERROR_LEN);           // 将数组置零
    av_strerror(err, m_error, ERROR_LEN);
    qWarning() << "VideoSave Error：" << m_error;
#else
    Q_UNUSED(err)
#endif
}

bool VideoSave::open(AVStream *inStream, const QString &fileName)
{
    if(!inStream || fileName.isEmpty()) return false;

    // 通过输出文件名为输出格式分配AVFormatContext。
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

    // 分配AVCodecContext并将其字段设置为默认值。
    m_codecContext = avcodec_alloc_context3(codec);
    if(!m_codecContext)
    {
        showError(AVERROR(ENOMEM));
        return false;
    }
    // 设置编码器上下文参数
    m_codecContext->width = inStream->codecpar->width;     // 图片宽度/高度
    m_codecContext->height = inStream->codecpar->height;
    m_codecContext->pix_fmt = codec->pix_fmts[0];          // 像素格式
    m_codecContext->time_base = {1, 20};                   //设置时间基，20为分母，1为分子，表示以1/20秒时间间隔播放一帧图像
    m_codecContext->framerate = {20, 1};
    m_codecContext->codec_type = codec->type;              // 编码器类型
    m_codecContext->bit_rate = 400000;                     //目标的码率，即采样的码率；显然，采样码率越大，视频大小越大

    // 打开编码器
    avcodec_open2(m_codecContext, codec, nullptr);
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

/**
 * @brief        写入数据
 * @param frame
 */
void VideoSave::write(AVFrame *frame)
{
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

/**
 * @brief 关闭保存数据
 */
void VideoSave::close()
{
    write(nullptr);   // 传入空帧，读取所有编码数据
    if(m_formatContext)
    {
        // 写入文件尾
        int ret = av_write_trailer(m_formatContext);
        if(ret < 0)
        {
            showError(ret);
            return;
        }
        ret = avio_close(m_formatContext->pb);
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
