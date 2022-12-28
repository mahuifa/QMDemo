#include "videodecode.h"
#include <QDebug>
#include <QImage>
#include <QMutex>
#include <qdatetime.h>


extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavdevice/avdevice.h"    // 调用输入设备需要的头文件
}

#define ERROR_LEN 1024  // 异常信息数组长度
#define PRINT_LOG 1

VideoDecode::VideoDecode()
{
    initFFmpeg();      // 5.1.2版本不需要调用了

    m_error = new char[ERROR_LEN];

    /**
     * dshow：  Windows 媒体输入设备。目前仅支持音频和视频设备。
     * gdigrab：基于 Win32 GDI 的屏幕捕获设备
     * video4linux2：Linux输入视频设备
     */
#if defined(Q_OS_WIN)
    m_inputFormat = av_find_input_format("gdigrab");            // Windows下如果没有则不能打开摄像头
#elif defined(Q_OS_LINUX)
    m_inputFormat = av_find_input_format("video4linux2");       // Linux也可以不需要就可以打开摄像头
#elif defined(Q_OS_MAC)
    m_inputFormat = av_find_input_format("avfoundation");
#endif

    if(!m_inputFormat)
    {
        qWarning() << "查询AVInputFormat失败！";
    }
}

VideoDecode::~VideoDecode()
{
    close();
}

/**
 * @brief 初始化ffmpeg库（整个程序中只需加载一次）
 *        旧版本的ffmpeg需要注册各种文件格式、解复用器、对网络库进行全局初始化。
 *        在新版本的ffmpeg中纷纷弃用了，不需要注册了
 */
void VideoDecode::initFFmpeg()
{
    static bool isFirst = true;
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if(isFirst)
    {
        //        av_register_all();         // 已经从源码中删除
        /**
         * 初始化网络库,用于打开网络流媒体，此函数仅用于解决旧GnuTLS或OpenSSL库的线程安全问题。
         * 一旦删除对旧GnuTLS和OpenSSL库的支持，此函数将被弃用，并且此函数不再有任何用途。
         */
        avformat_network_init();
        // 初始化libavdevice并注册所有输入和输出设备。
        avdevice_register_all();
        isFirst = false;
    }
}

/**
 * @brief      打开媒体文件，或者流媒体，例如rtmp、strp、http
 * @param url  视频地址
 * @return     true：成功  false：失败
 */
bool VideoDecode::open(const QString &url)
{
    if(url.isNull()) return false;

    AVDictionary* dict = nullptr;
    av_dict_set(&dict, "framerate", "20", 0);          // 设置帧率，默认的是30000/1001，但是实际可能达不到30的帧率，所以最好手动设置
    av_dict_set(&dict, "draw_mouse", "1", 0);          // 指定是否绘制鼠标指针。0：不包含鼠标，1：包含鼠标
//    av_dict_set(&dict, "video_size", "500x400", 0);    // 录制视频的大小（宽高），默认为全屏
//    av_dict_set(&dict, "offset_x", "100", 0);          // 录制视频的起点X坐标
//    av_dict_set(&dict, "offset_y", "500", 0);          // 录制视频的起点Y坐标

    // 打开输入流并返回解封装上下文
    int ret = avformat_open_input(&m_formatContext,          // 返回解封装上下文
                                  url.toStdString().data(),  // 打开视频地址
                                  m_inputFormat,             // 如果非null，此参数强制使用特定的输入格式。自动选择解封装器（文件格式）
                                  &dict);                    // 参数设置

    // 释放参数字典
    if(dict)
    {
        av_dict_free(&dict);
    }
    // 打开视频失败
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }

    // 读取媒体文件的数据包以获取流信息。
    ret = avformat_find_stream_info(m_formatContext, nullptr);
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }
    m_totalTime = m_formatContext->duration / (AV_TIME_BASE / 1000); // 计算视频总时长（毫秒）
#if PRINT_LOG
    qDebug() << QString("视频总时长：%1 ms，[%2]").arg(m_totalTime).arg(QTime::fromMSecsSinceStartOfDay(int(m_totalTime)).toString("HH:mm:ss zzz"));
#endif

    // 通过AVMediaType枚举查询视频流ID（也可以通过遍历查找），最后一个参数无用
    m_videoIndex = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if(m_videoIndex < 0)
    {
        showError(m_videoIndex);
        free();
        return false;
    }

    AVStream* videoStream = m_formatContext->streams[m_videoIndex];  // 通过查询到的索引获取视频流

    // 获取视频图像分辨率（AVStream中的AVCodecContext在新版本中弃用，改为使用AVCodecParameters）
    m_size.setWidth(videoStream->codecpar->width);
    m_size.setHeight(videoStream->codecpar->height);
    m_frameRate = rationalToDouble(&videoStream->avg_frame_rate);  // 视频帧率
    m_avgFrameRate.setX(videoStream->avg_frame_rate.num);
    m_avgFrameRate.setY(videoStream->avg_frame_rate.den);

    // 通过解码器ID获取视频解码器（新版本返回值必须使用const）
    const AVCodec* codec = avcodec_find_decoder(videoStream->codecpar->codec_id);
    m_totalFrames = videoStream->nb_frames;

#if PRINT_LOG
    qDebug() << QString("分辨率：[w:%1,h:%2] 帧率：%3  总帧数：%4  解码器：%5")
                .arg(m_size.width()).arg(m_size.height()).arg(m_frameRate).arg(m_totalFrames).arg(codec->name);
#endif

    // 分配AVCodecContext并将其字段设置为默认值。
    m_codecContext = avcodec_alloc_context3(codec);
    if(!m_codecContext)
    {
#if PRINT_LOG
        qWarning() << "创建视频解码器上下文失败！";
#endif
        free();
        return false;
    }

    // 使用视频流的codecpar为解码器上下文赋值
    ret = avcodec_parameters_to_context(m_codecContext, videoStream->codecpar);
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }

    m_codecContext->flags2 |= AV_CODEC_FLAG2_FAST;    // 允许不符合规范的加速技巧。
    m_codecContext->thread_count = 8;                 // 使用8线程解码

    // 初始化解码器上下文，如果之前avcodec_alloc_context3传入了解码器，这里设置NULL就可以
    ret = avcodec_open2(m_codecContext, nullptr, nullptr);
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }

    // 分配AVPacket并将其字段设置为默认值。
    m_packet = av_packet_alloc();
    if(!m_packet)
    {
#if PRINT_LOG
        qWarning() << "av_packet_alloc() Error！";
#endif
        free();
        return false;
    }
    // 分配AVFrame并将其字段设置为默认值。
    m_frame = av_frame_alloc();
    m_frameY = av_frame_alloc();
    if(!m_frame || !m_frameY)
    {
#if PRINT_LOG
        qWarning() << "av_frame_alloc() Error！";
#endif
        free();
        return false;
    }

    // 创建一个图像帧用于保存YUV420P图像
    m_frameY->width = m_size.width();
    m_frameY->height = m_size.height();
    m_frameY->format = AV_PIX_FMT_YUV420P;
    av_frame_get_buffer(m_frameY, 3 * 8);

    m_end = false;

    return true;
}

/**
 * @brief   读取图像并将图像转换为YUV420P格式
 * @return
 */
AVFrame* VideoDecode::read()
{
    // 如果没有打开则返回
    if(!m_formatContext)
    {
        return nullptr;
    }

    // 读取下一帧数据
    int readRet = av_read_frame(m_formatContext, m_packet);
    if(readRet < 0)
    {
        avcodec_send_packet(m_codecContext, m_packet); // 读取完成后向解码器中传如空AVPacket，否则无法读取出最后几帧
    }
    else
    {

        if(m_packet->stream_index == m_videoIndex)     // 如果是图像数据则进行解码
        {
            // 计算当前帧时间（毫秒）
#if 1       // 方法一：适用于所有场景，但是存在一定误差
            m_packet->pts = qRound64(m_packet->pts * (1000 * rationalToDouble(&m_formatContext->streams[m_videoIndex]->time_base)));
            m_packet->dts = qRound64(m_packet->dts * (1000 * rationalToDouble(&m_formatContext->streams[m_videoIndex]->time_base)));
#else       // 方法二：适用于播放本地视频文件，计算每一帧时间较准，但是由于网络视频流无法获取总帧数，所以无法适用
            m_obtainFrames++;
            m_packet->pts = qRound64(m_obtainFrames * (qreal(m_totalTime) / m_totalFrames));
#endif
            // 将读取到的原始数据包传入解码器
            int ret = avcodec_send_packet(m_codecContext, m_packet);
            if(ret < 0)
            {
                showError(ret);
            }
        }
    }
    av_packet_unref(m_packet);  // 释放数据包，引用计数-1，为0时释放空间

    av_frame_unref(m_frame);
    int ret = avcodec_receive_frame(m_codecContext, m_frame);
    if(ret < 0)
    {
        av_frame_unref(m_frame);
        if(readRet < 0)
        {
            m_end = true;     // 当无法读取到AVPacket并且解码器中也没有数据时表示读取完成
        }
        return nullptr;
    }

    if(m_frame->format == AV_PIX_FMT_YUV420P)   // 如果图像格式为YUV420P则直接返回，不进行格式转换
    {
        return m_frame;
    }

    // 为什么图像转换上下文要放在这里初始化呢，是因为m_frame->format，如果使用硬件解码，解码出来的图像格式和m_codecContext->pix_fmt的图像格式不一样，就会导致无法转换为QImage
    // 由于解码后的图像格式不一定支持保存裸流，或者不支持直接编码为H264，所以需要转换格式
    if(!m_swsContext)
    {
        // 获取缓存的图像转换上下文。首先校验参数是否一致，如果校验不通过就释放资源；然后判断上下文是否存在，如果存在直接复用，如不存在进行分配、初始化操作
        m_swsContext = sws_getCachedContext(m_swsContext,
                                            m_frame->width,                     // 输入图像的宽度
                                            m_frame->height,                    // 输入图像的高度
                                            (AVPixelFormat)m_frame->format,     // 输入图像的像素格式
                                            m_size.width(),                     // 输出图像的宽度
                                            m_size.height(),                    // 输出图像的高度
                                            AV_PIX_FMT_YUV420P,                    // 输出图像的像素格式
                                            SWS_BILINEAR,                       // 选择缩放算法(只有当输入输出图像大小不同时有效),一般选择SWS_FAST_BILINEAR
                                            nullptr,                            // 输入图像的滤波器信息, 若不需要传NULL
                                            nullptr,                            // 输出图像的滤波器信息, 若不需要传NULL
                                            nullptr);                          // 特定缩放算法需要的参数(?)，默认为NULL
        if(!m_swsContext)
        {
#if PRINT_LOG
            qWarning() << "sws_getCachedContext() Error！";
#endif
            free();
            return nullptr;
        }
    }

    ret = sws_scale(m_swsContext,             // 缩放上下文
                    m_frame->data,            // 原图像数组
                    m_frame->linesize,        // 包含源图像每个平面步幅的数组
                    0,                        // 开始位置
                    m_frame->height,          // 行数
                    m_frameY->data,                     // 目标图像数组
                    m_frameY->linesize);                   // 包含目标图像每个平面的步幅的数组
    av_frame_unref(m_frame);
    return m_frameY;
}

/**
 * @brief 关闭视频播放并释放内存
 */
void VideoDecode::close()
{
    clear();
    free();

    m_totalTime     = 0;
    m_videoIndex    = 0;
    m_totalFrames   = 0;
    m_obtainFrames  = 0;
    m_frameRate     = 0;
    m_size          = QSize(0, 0);
}

/**
 * @brief  视频是否读取完成
 * @return
 */
bool VideoDecode::isEnd()
{
    return m_end;
}


/**
 * @brief        显示ffmpeg函数调用异常信息
 * @param err
 */
void VideoDecode::showError(int err)
{
#if PRINT_LOG
    memset(m_error, 0, ERROR_LEN);        // 将数组置零
    av_strerror(err, m_error, ERROR_LEN);
    qWarning() << "DecodeVideo Error：" << m_error;
#else
    Q_UNUSED(err)
#endif
}

/**
 * @brief          将AVRational转换为double，用于计算帧率
 * @param rational
 * @return
 */
qreal VideoDecode::rationalToDouble(AVRational* rational)
{
    qreal frameRate = (rational->den == 0) ? 0 : (qreal(rational->num) / rational->den);
    return frameRate;
}

/**
 * @brief 清空读取缓冲
 */
void VideoDecode::clear()
{
    // 因为avformat_flush不会刷新AVIOContext (s->pb)。如果有必要，在调用此函数之前调用avio_flush(s->pb)。
    if(m_formatContext && m_formatContext->pb)
    {
        avio_flush(m_formatContext->pb);
    }
    if(m_formatContext)
    {
        avformat_flush(m_formatContext);   // 清理读取缓冲
    }
}

void VideoDecode::free()
{
    // 释放上下文swsContext。
    if(m_swsContext)
    {
        sws_freeContext(m_swsContext);
        m_swsContext = nullptr;             // sws_freeContext不会把上下文置NULL
    }
    // 释放编解码器上下文和与之相关的所有内容，并将NULL写入提供的指针
    if(m_codecContext)
    {
        avcodec_free_context(&m_codecContext);
    }
    // 关闭并失败m_formatContext，并将指针置为null
    if(m_formatContext)
    {
        avformat_close_input(&m_formatContext);
    }
    if(m_packet)
    {
        av_packet_free(&m_packet);
    }
    if(m_frame)
    {
        av_frame_free(&m_frame);
    }
    if(m_frameY)
    {
        av_frame_unref(m_frameY);
        av_frame_free(&m_frameY);
    }
}
