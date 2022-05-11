#include "abstractthread.h"
#include <QThread>
#include <QDebug>

AbstractThread::AbstractThread(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_thread->start();

    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);	// 退出后释放QThread对象资源
    connect(this, &AbstractThread::inputData, this, &AbstractThread::handleData);


    qDebug() << "创建一个AbstractThread";
}

AbstractThread::~AbstractThread()
{
#if 1
    m_thread->quit();         // 通知线程退出
    m_thread->wait();         // 等待线程退出
#else
    m_thread->exit();         // 这里方式如果线程没有立刻退出则会报错
    delete  m_thread;         // 直接关闭线程
#endif
    m_thread = nullptr;

    qDebug() << "~AbstractThread()";
}
