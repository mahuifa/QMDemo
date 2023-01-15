#include "work1.h"

Work1::Work1(QObject *parent) : QObject(parent)
{
    // 在内部创建子线程
    m_thread = new WorkThread();   // 这里通过继承QThread验证在程序退出时是否会释放m_thread
    m_thread->start();
    this->moveToThread(m_thread);
    connect(this, &Work1::start, this, &Work1::delayCount);
}

Work1::~Work1()
{
    m_thread->quit();           // 告诉线程的事件循环退出
    m_thread->wait();           // 等待线程执行完成(默认不会超时，一直等到线程退出)
//    qDebug() << m_thread->wait(1000);           // 等待1000毫秒，如果线程没有退出则超时，直接终止线程，返回false
//    delete m_thread;          // 由于m_thread没有指定父对象，所以需要自己释放
//    m_thread = nullptr;
    qDebug() << "析构Work1";
}

void Work1::delayCount()
{
    emit updateData(10);
    QThread::sleep(5);  // 延时5秒，模拟耗时操作
    emit updateData(100);
}
