#include "readthread.h"
#include "videodecode.h"

ReadThread::ReadThread(QObject *parent) : QThread(parent)
{
    m_videoDecode = new VideoDecode();
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

void ReadThread::pause(bool flag)
{

}

void ReadThread::close()
{

}

const QString &ReadThread::url()
{
    QString url;
    return url;
}

void ReadThread::run()
{
    bool ret = m_videoDecode->open(m_url);
}
