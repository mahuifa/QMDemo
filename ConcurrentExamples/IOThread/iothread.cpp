#include "iothread.h"

IOThread::IOThread(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_thread->start();   // 创建子线程

    connect(m_thread, &QThread::started, this, &IOThread::on_started);
    connect(m_thread, &QThread::finished, this, &IOThread::on_finished);
}

IOThread::~IOThread()
{
    // 退出子线程
    m_thread->quit();
    m_thread->wait();
}

/**
 * @brief 线程开始执行时触发的函数，所有需要在子线程中操作的QIODevice都要在这里初始化
 */
void IOThread::on_started()
{
    m_client = new QTcpSocket();
}

/**
 * @brief 线程退出时触发的函数，子线程中的对象需要在这里释放
 */
void IOThread::on_finished()
{
    if(m_client)
    {
        m_client->close();
        delete m_client;
        m_client = nullptr;
    }
}
