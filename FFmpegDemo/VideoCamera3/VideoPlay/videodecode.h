/******************************************************************************
 * @文件名     videodecode.h
 * @功能       视频解码类，在这个类中调用ffmpeg打开摄像头获取图像数据
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/09/15
 * @备注
 *****************************************************************************/
#ifndef VIDEODECODE_H
#define VIDEODECODE_H

#include <QSize>
#include <QString>

struct AVFormatContext;
struct AVCodecContext;
struct AVRational;
struct AVPacket;
struct AVFrame;
struct SwsContext;
struct AVBufferRef;
struct AVInputFormat;
class QImage;

class VideoDecode
{
public:
    VideoDecode();
    ~VideoDecode();

    bool open(const QString& url = QString());   // 打开媒体文件，或者流媒体rtmp、strp、http
    AVFrame* read();                             // 读取视频图像
    void close();                                // 关闭

private:
    void initFFmpeg();                              // 初始化ffmpeg库（整个程序中只需加载一次）
    void showError(int err);                        // 显示ffmpeg执行错误时的错误信息
    qreal rationalToDouble(AVRational* rational);   // 将AVRational转换为double
    bool toYUV420P();                               // 将视频帧格式由原始格式转换为YUV420P格式，便于显示
    void clear();                                   // 清空读取缓冲
    void free();                                    // 释放

private:
    const AVInputFormat* m_inputFormat = nullptr;
    AVFormatContext* m_formatContext = nullptr;   // 解封装上下文
    AVCodecContext* m_codecContext = nullptr;     // 解码器上下文
    SwsContext* m_swsContext = nullptr;           // 图像转换上下文
    AVPacket* m_packet = nullptr;                 // 数据包
    AVFrame* m_frame = nullptr;                   // 解码后的视频帧（转换为YUV420P格式）
    int m_videoIndex = 0;                         // 视频流索引
    qint64 m_totalFrames = 0;                     // 视频总帧数
    qreal m_frameRate = 0;                        // 视频帧率
    char* m_error = nullptr;                      // 保存异常信息
    QSize m_size;
};

#endif   // VIDEODECODE_H
