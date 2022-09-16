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

struct AVFormatContext;
struct AVCodecContext;
struct AVRational;
struct AVPacket;
struct AVFrame;
struct SwsContext;
struct AVBufferRef;
class QImage;

class VideoDecode
{
public:
    VideoDecode();
    ~VideoDecode();

    bool open(const QString& url = QString());    // 打开媒体文件，或者流媒体rtmp、strp、http
    QImage *read();                               // 读取视频图像
    void close();                                 // 关闭
    bool isEnd();                                 // 是否读取完成
    const qint64& pts();                          // 获取当前帧显示时间

private:
    void initFFmpeg();                            // 初始化ffmpeg库（整个程序中只需加载一次）
    void showError(int err);                      // 显示ffmpeg执行错误时的错误信息
    qreal rationalToDouble(AVRational rational);  // 将AVRational转换为double
    void clear();                                 // 清空读取缓冲
    void free();                                  // 释放
};

#endif // VIDEODECODE_H
