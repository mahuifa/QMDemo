#include "workthread.h"
#include <QDebug>
WorkThread::WorkThread(QObject *parent) : QThread(parent)
{
}

WorkThread::~WorkThread()
{
    qDebug() << "析构线程";
    m_abort = true;      // 设置将线程退出标志
    this->wait();        // 等待线程退出
}

/**
 * @brief 退出线程
 */
void WorkThread::abort()
{
    m_abort = true;
}

void WorkThread::run()
{
    qDebug() << "开始线程";
    m_abort = false;
    int i = 0;
    forever
    {
        QThread::sleep(1);
        emit updateData(++i);

        // 退出线程
        if(m_abort) break;
    }

    qDebug() << "退出线程";
}
