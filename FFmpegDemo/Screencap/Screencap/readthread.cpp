#include "readthread.h"
#include "videodecode.h"
#include "videocodec.h"

#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <qimage.h>
#include <QFileInfo>
#include <QMessageBox>

ReadThread::ReadThread(QObject *parent) : QThread(parent)
{
    m_videoDecode = new VideoDecode();
    m_videoCodec  = new VideoCodec();

    qRegisterMetaType<PlayState>("PlayState");    // 注册自定义枚举类型，否则信号槽无法发送
}

ReadThread::~ReadThread()
{
    if(m_videoDecode)
    {
        delete m_videoDecode;
    }
}

/**
 * @brief       设置视频保存地址
 * @param path
 */
void ReadThread::setPath(const QString &path)
{
    if(path.isEmpty()) return;

    if(QFileInfo(path).suffix().isEmpty())
    {
        QMessageBox::warning(nullptr, "注意~", "输入文件没有后缀名，无法使用");
        m_path.clear();
        return;
    }
    m_path = path;
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
    if(m_path.isEmpty()) return;

    bool ret = m_videoDecode->open(m_url);         // 打开网络流时会比较慢，如果放到Ui线程会卡
    if(ret)
    {
        m_play = m_videoCodec->open(m_videoDecode->getCodecContext(), m_videoDecode->avgFrameRate(), m_path);
        if(!m_play)
        {
            qDebug() << "打开输出文件失败！";
        }
        else
        {
            emit playState(play);
        }
    }
    else
    {
        qWarning() << "打开失败！";
    }
    // 循环读取视频图像
    while (m_play)
    {
        AVFrame* frame = m_videoDecode->read();  // 读取视频图像
        if(frame)
        {
            m_videoCodec->write(frame);          // 将解码后的图像数据传递给编码保存类
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

    qDebug() << "录屏结束！";
    m_videoDecode->close();
    m_videoCodec->close();
    emit playState(end);
}
