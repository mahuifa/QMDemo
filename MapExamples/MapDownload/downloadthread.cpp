#include "downloadthread.h"

#include <QThread>

DownloadThread::DownloadThread(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_thread->start();
}

DownloadThread::~DownloadThread()
{
    m_thread->quit();
    m_thread->wait();
}
