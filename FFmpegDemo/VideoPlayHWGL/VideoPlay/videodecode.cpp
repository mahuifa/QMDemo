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
}

#define ERROR_LEN 1024  // 异常信息数组长度
#define PRINT_LOG 1

VideoDecode::VideoDecode()
{
//    initFFmpeg();      // 5.1.2版本不需要调用了

    m_error = new char[ERROR_LEN];

    /*************************************** 获取当前环境支持的硬件解码器 *********************************************/
    AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE;      // ffmpeg支持的硬件解码器
    QStringList strTypes;
    while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)       // 遍历支持的设备类型。
    {
        m_HWDeviceTypes.append(type);
        const char* ctype = av_hwdevice_get_type_name(type);  // 获取AVHWDeviceType的字符串名称。
        if(ctype)
        {
            strTypes.append(QString(ctype));
        }
    }
    qDebug() << "支持的硬件解码器：" << strTypes;
    /************************************************ END ******************************************************/
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
        isFirst = false;
    }
}

/*********************************** FFmpeg获取GPU硬件解码帧格式的回调函数 *****************************************/
static enum AVPixelFormat g_pixelFormat;
/**
 * @brief      回调函数，获取GPU硬件解码帧的格式
 * @param s
 * @param fmt
 * @return
 */
AVPixelFormat get_hw_format(AVCodecContext* s, const enum AVPixelFormat* fmt)
{
    Q_UNUSED(s)
    const enum AVPixelFormat* p;

    for (p = fmt; *p != -1; p++)
    {
        if(*p == g_pixelFormat)
        {
            return *p;
        }
    }

    qDebug() << "无法获取硬件表面格式.";         // 当同时打开太多路视频时，如果超过了GPU的能力，可能会返回找不到解码帧格式
    return AV_PIX_FMT_NONE;
}
/************************************************ END ******************************************************/

/**************************************** FFmpeg初始化硬件解码器 **********************************************/
/**
 * @brief         初始化硬件解码器
 * @param codec
 */
void VideoDecode::initHWDecoder(const AVCodec *codec)
{
    if(!codec) return;

    for(int i = 0; ; i++)
    {
        const AVCodecHWConfig* config = avcodec_get_hw_config(codec, i);    // 检索编解码器支持的硬件配置。
        if(!config)
        {
            qDebug() << "打开硬件解码器失败！";
            return;          // 没有找到支持的硬件配置
        }

        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX)       // 判断是否是设备类型
        {
            for(auto i : m_HWDeviceTypes)
            {
                if(config->device_type == AVHWDeviceType(i))                 // 判断设备类型是否是支持的硬件解码器
                {
                    g_pixelFormat = config->pix_fmt;

                    // 打开指定类型的设备，并为其创建AVHWDeviceContext。
                    int ret = av_hwdevice_ctx_create(&hw_device_ctx, config->device_type, nullptr, nullptr, 0);
                    if(ret < 0)
                    {
                        showError(ret);
                        free();
                        return ;
                    }
                    qDebug() << "打开硬件解码器：" << av_hwdevice_get_type_name(config->device_type);
                    m_codecContext->hw_device_ctx = av_buffer_ref(hw_device_ctx);  // 创建一个对AVBuffer的新引用。
                    m_codecContext->get_format = get_hw_format;                    // 由一些解码器调用，以选择将用于输出帧的像素格式
                    return;
                }
            }
        }
    }
}

/************************************************ END ******************************************************/

/**
 * @brief      打开媒体文件，或者流媒体，例如rtmp、strp、http
 * @param url  视频地址
 * @return     true：成功  false：失败
 */
bool VideoDecode::open(const QString &url)
{
    if(url.isNull()) return false;

    AVDictionary* dict = nullptr;
    av_dict_set(&dict, "rtsp_transport", "tcp", 0);      // 设置rtsp流使用tcp打开，如果打开失败错误信息为【Error number -135 occurred】可以切换（UDP、tcp、udp_multicast、http），比如vlc推流就需要使用udp打开
    av_dict_set(&dict, "max_delay", "3", 0);             // 设置最大复用或解复用延迟（以微秒为单位）。当通过【UDP】 接收数据时，解复用器尝试重新排序接收到的数据包（因为它们可能无序到达，或者数据包可能完全丢失）。这可以通过将最大解复用延迟设置为零（通过max_delayAVFormatContext 字段）来禁用。
    av_dict_set(&dict, "timeout", "1000000", 0);         // 以微秒为单位设置套接字 TCP I/O 超时，如果等待时间过短，也可能会还没连接就返回了。

    // 打开输入流并返回解封装上下文
    int ret = avformat_open_input(&m_formatContext,          // 返回解封装上下文
                                  url.toStdString().data(),  // 打开视频地址
                                  nullptr,                   // 如果非null，此参数强制使用特定的输入格式。自动选择解封装器（文件格式）
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

    if(m_HWDecoder)
    {
        initHWDecoder(codec);     // 初始化硬件解码器（在avcodec_open2前调用）
    }

    // 初始化解码器上下文，如果之前avcodec_alloc_context3传入了解码器，这里设置NULL就可以
    ret = avcodec_open2(m_codecContext, nullptr, nullptr);
    if(ret < 0)
    {
        showError(ret);
        free();
        return false;
    }

    return initObject();
}

/**
 * @brief   初始化需要用到的对象
 * @return
 */
bool VideoDecode::initObject()
{
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
    if(!m_frame)
    {
#if PRINT_LOG
        qWarning() << "av_frame_alloc() Error！";
#endif
        free();
        return false;
    }
    m_frameHW = av_frame_alloc();
    if(!m_frameHW)
    {
#if PRINT_LOG
        qWarning() << "av_frame_alloc() Error！";
#endif
        free();
        return false;
    }


    // 由于传递时是浅拷贝，可能显示类还没处理完成，所以如果播放完成就释放可能会崩溃；
    if(m_buffer)
    {
        delete [] m_buffer;
        m_buffer = nullptr;
    }
    // 分配图像空间
    int size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_size.width(), m_size.height(), 4);
    /**
     * 【注意：】这里可以多分配一些，否则如果只是安装size分配，大部分视频图像数据拷贝没有问题，
     *         但是少部分视频图像在使用sws_scale()拷贝时会超出数组长度，在使用使用msvc debug模式时delete[] m_buffer会报错（HEAP CORRUPTION DETECTED: after Normal block(#32215) at 0x000001AC442830370.CRT delected that the application wrote to memory after end of heap buffer）
     *         特别是这个视频流http://vfx.mtime.cn/Video/2019/02/04/mp4/190204084208765161.mp4
     */
    m_buffer = new uchar[size + 1000];    // 这里多分配1000个字节就基本不会出现拷贝超出的情况了，反正不缺这点内存
//    m_image = new QImage(m_buffer, m_size.width(), m_size.height(), QImage::Format_RGBA8888);  // 这种方式分配内存大部分情况下也可以，但是因为存在拷贝超出数组的情况，delete时也会报错
    m_end = false;

    return true;
}


/**
 * @brief    读取并返回视频图像
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
    av_frame_unref(m_frameHW);
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

    // 这样写是为了兼容软解码或者硬件解码打开失败情况
    AVFrame*  m_frameTemp = m_frame;
    if(!m_frame->data[0])               // 如果是硬件解码就进入
    {
        m_frameTemp = m_frameHW;
        // 将解码后的数据从GPU拷贝到CPU
        if(!dataCopy())
        {
            return nullptr;
        }
    }

    m_pts = m_frameTemp->pts;

    return m_frameTemp;
}

/********************************* FFmpeg初始化硬件后将图像数据从GPU拷贝到CPU *************************************/
/**
 * @brief   硬件解码完成需要将数据从GPU复制到CPU
 * @return
 */
bool VideoDecode::dataCopy()
{
    if(m_frame->format != g_pixelFormat)
    {
        av_frame_unref(m_frame);
        return false;
    }
#if 1   // av_hwframe_map在ffmpeg3.3以后才有，经过测试av_hwframe_transfer_data的耗时大概是av_hwframe_map的【1.5倍】
    int ret = av_hwframe_map(m_frameHW, m_frame, 0);                   // 映射硬件数据帧
    if(ret < 0)
    {
        showError(ret);
        av_frame_unref(m_frame);
        return false;
    }
    m_frameHW->width = m_frame->width;
    m_frameHW->height = m_frame->height;
#else
    int ret = av_hwframe_transfer_data(m_frameHW, m_frame, 0);       // 将解码后的数据从GPU复制到CPU(m_frameHW) 这一步比较耗时，在这一步之前硬解码速度比软解码快很多
    if(ret < 0)
    {
        showError(ret);
        av_frame_unref(m_frame);
        return false;
    }
    av_frame_copy_props(m_frameHW, m_frame);                        // 仅将“metadata”字段从src复制到dst。
#endif
    return true;
}

/************************************************ END ******************************************************/

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
    m_pts           = 0;
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
 * @brief    返回当前帧图像播放时间
 * @return
 */
const qint64 &VideoDecode::pts()
{
    return m_pts;
}

/**
 * @brief         设置是否使用硬件解码
 * @param flag    true：使用 false：不使用
 */
void VideoDecode::setHWDecoder(bool flag)
{
    m_HWDecoder = flag;
}

/**
 * @brief   返回当前是否使用硬件解码
 * @return
 */
bool VideoDecode::isHWDecoder()
{
    return m_HWDecoder;
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
    if(hw_device_ctx)
    {
        av_buffer_unref(&hw_device_ctx);
    }
    if(m_packet)
    {
        av_packet_free(&m_packet);
    }
    if(m_frame)
    {
        av_frame_free(&m_frame);
    }
    if(m_frameHW)
    {
        av_frame_free(&m_frameHW);
    }
}
