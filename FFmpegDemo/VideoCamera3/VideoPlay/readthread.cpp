#include "readthread.h"
#include "videodecode.h"

#include <QVariant>

#include <playimage.h>
#include <qimage.h>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

extern "C"
{   // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

ReadThread::ReadThread(QObject* parent)
    : QThread(parent)
{
    m_videoDecode = new VideoDecode();

    // 注册自定义枚举类型，否则信号槽无法发送
    qRegisterMetaType<PlayState>("PlayState");
}

ReadThread::~ReadThread()
{
    if (m_videoDecode)
    {
        delete m_videoDecode;
    }
}

/**
 * @brief      传入播放的视频地址并开启线程
 * @param url
 */
void ReadThread::open(const QString& url)
{
    if (!this->isRunning())
    {
        m_url = url;
        emit this->start();
    }
}

/**
 * @brief 关闭播放
 */
void ReadThread::close()
{
    m_play = false;
}

/**
 * @brief    返回当前播放的地址
 * @return
 */
const QString& ReadThread::url()
{
    return m_url;
}

/**
 * @brief      非阻塞延时
 * @param msec 延时毫秒
 */
void sleepMsec(int msec)
{
    if (msec <= 0)
        return;
    QEventLoop loop;                                 //定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));   //创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();                                     //事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void ReadThread::run()
{
    bool ret = m_videoDecode->open(m_url);   // 打开网络流时会比较慢，如果放到Ui线程会卡
    if (ret)
    {
        m_play = true;
        emit playState(play);
    }
    else
    {
        qWarning() << "打开失败！";
    }
    // 循环读取视频图像
    while (m_play)
    {
        AVFrame* frame = m_videoDecode->read();   // 读取视频图像
        if (frame)
        {
            emit repaint(frame);
        }
        else
        {
            sleepMsec(1);   // 这里不能使用QThread::msleep()延时，否则会很不稳定
        }
    }

    qDebug() << "播放结束！";
    m_videoDecode->close();
    emit playState(end);
}
