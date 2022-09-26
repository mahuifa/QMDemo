/******************************************************************************
 * @文件名     videodecode.h
 * @功能       视频解码类，在这个类中调用ffmpeg打开视频进行解码
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/09/15
 * @备注
 *****************************************************************************/
#ifndef VIDEODECODE_H
#define VIDEODECODE_H

#include <QString>
#include <QSize>
#include <qlist.h>

struct AVFormatContext;
struct AVCodecContext;
struct AVRational;
struct AVPacket;
struct AVFrame;
struct AVCodec;
struct SwsContext;
struct AVBufferRef;
class QImage;

class VideoDecode
{
public:
    VideoDecode();
    ~VideoDecode();

    bool open(const QString& url = QString());    // 打开媒体文件，或者流媒体rtmp、strp、http
    QImage read();                                // 读取视频图像
    void close();                                 // 关闭
    bool isEnd();                                 // 是否读取完成
    const qint64& pts();                          // 获取当前帧显示时间
    void setHWDecoder(bool flag);                 // 是否使用硬件解码器
    bool isHWDecoder();

private:
    void initFFmpeg();                            // 初始化ffmpeg库（整个程序中只需加载一次）
    void initHWDecoder(const AVCodec* codec);     // 初始化硬件解码器
    bool initObject();                            // 初始化对象
    bool dataCopy();                              // 硬件解码完成需要将数据从GPU复制到CPU
    void showError(int err);                      // 显示ffmpeg执行错误时的错误信息
    qreal rationalToDouble(AVRational* rational); // 将AVRational转换为double
    void clear();                                 // 清空读取缓冲
    void free();                                  // 释放

private:
    AVFormatContext* m_formatContext = nullptr;   // 解封装上下文
    AVCodecContext*  m_codecContext  = nullptr;   // 解码器上下文
    SwsContext*      m_swsContext    = nullptr;   // 图像转换上下文
    AVPacket* m_packet = nullptr;                 // 数据包
    AVFrame*  m_frame  = nullptr;                 // 解码后的视频帧
    AVFrame*  m_frameHW = nullptr;                // 硬件解码后的视频帧
    int    m_videoIndex   = 0;                    // 视频流索引
    qint64 m_totalTime    = 0;                    // 视频总时长
    qint64 m_totalFrames  = 0;                    // 视频总帧数
    qint64 m_obtainFrames = 0;                    // 视频当前获取到的帧数
    qint64 m_pts          = 0;                    // 图像帧的显示时间
    qreal  m_frameRate    = 0;                    // 视频帧率
    QSize  m_size;                                // 视频分辨率大小
    char*  m_error = nullptr;                     // 保存异常信息
    bool   m_end = false;                         // 视频读取完成
    uchar* m_buffer = nullptr;

    QList<int> m_HWDeviceTypes;                   // 保存当前环境支持的硬件解码器
    AVBufferRef* hw_device_ctx = nullptr;         // 对数据缓冲区的引用
    bool   m_HWDecoder = false;                   // 记录是否使用硬件解码
};

#endif // VIDEODECODE_H
