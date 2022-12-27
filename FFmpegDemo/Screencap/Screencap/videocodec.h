/******************************************************************************
 * @文件名     videocodec.h
 * @功能       视频编码保存类
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/12/26
 * @备注
 *****************************************************************************/
#ifndef VIDEOCODEC_H
#define VIDEOCODEC_H

#include <QPoint>
#include <qmutex.h>
#include <qstring.h>


struct AVCodecParameters;
struct AVFormatContext;
struct AVCodecContext;
struct AVStream;
struct AVFrame;
struct AVPacket;
struct AVOutputFormat;
class VideoCodec
{
public:
    VideoCodec();
    ~VideoCodec();

    bool open(AVCodecContext *codecContext, QPoint point, const QString& fileName);
    void write(AVFrame* frame);
    void close();

private:
    void showError(int err);

private:
    AVFormatContext* m_formatContext = nullptr;
    AVCodecContext * m_codecContext  = nullptr;    // 编码器上下文
    AVStream       * m_videoStream   = nullptr;
    AVPacket       * m_packet        = nullptr;    // 数据包
    int m_index = 0;
    bool             m_writeHeader   = false;      // 是否写入头
    QMutex           m_mutex;
};

#endif // VIDEOCODEC_H
