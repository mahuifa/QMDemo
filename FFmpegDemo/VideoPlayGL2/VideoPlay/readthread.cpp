#include "readthread.h"
#include "videodecode.h"

#include<QVariant>

#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <qimage.h>
#include <playimage.h>

extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

ReadThread::ReadThread(QObject *parent) : QThread(parent)
{
    m_videoDecode = new VideoDecode();

    // 注册自定义枚举类型，否则信号槽无法发送
    qRegisterMetaType<PlayState>("PlayState");
}

ReadThread::~ReadThread()
{
    if(m_videoDecode)
    {
        delete m_videoDecode;
    }
}

/**
 * @brief      传入播放的视频地址并开启线程
 * @param url
 */
void ReadThread::open(const QString &url)
{
    if(!this->isRunning())
    {
        m_url = url;
        emit this->start();
    }
}

/**
 * @brief       控制暂停、继续
 * @param flag  true：暂停  fasle：继续
 */
void ReadThread::pause(bool flag)
{
    m_pause = flag;
}

/**
 * @brief 关闭播放
 */
void ReadThread::close()
{
    m_play = false;
    m_pause = false;
}

/**
 * @brief    返回当前播放的地址
 * @return
 */
const QString &ReadThread::url()
{
    return m_url;
}

/**
 * @brief      非阻塞延时
 * @param msec 延时毫秒
 */
void  sleepMsec(int msec)
{
    if(msec <= 0) return;
    QEventLoop loop;		//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();			//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void ReadThread::run()
{
    bool ret = m_videoDecode->open(m_url);         // 打开网络流时会比较慢，如果放到Ui线程会卡
    if(ret)
    {
        m_play = true;
        m_etime1.start();
        m_etime2.start();
        emit playState(play);
    }
    else
    {
        qWarning() << "打开失败！";
    }
    // 循环读取视频图像
    while (m_play)
    {
        // 暂停
        while (m_pause)
        {
            sleepMsec(200);
        }

        AVFrame* frame = m_videoDecode->read();  // 读取视频图像
        if(frame)
        {
            // 1倍速播放
#if 0
            sleepMsec(int(m_decodeVideo->pts() - m_etime1.elapsed()));         // 不支持后退
#else
            sleepMsec(int(m_videoDecode->pts() - m_etime2.elapsed()));         // 支持后退（如果能读取到视频，但一直不显示可以把这一行代码注释试试）
#endif
            emit repaint(frame);
        }
        else
        {
            // 当前读取到无效图像时判断是否读取完成
            if(m_videoDecode->isEnd())
            {
                break;
            }
            sleepMsec(1);   // 这里不能使用QThread::msleep()延时，否则会很不稳定
        }
    }

    qDebug() << "播放结束！";
    m_videoDecode->close();
    emit playState(end);
}
