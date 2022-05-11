#include "handletcpthread.h"

#include <qdebug.h>
#include <qthread.h>

handleTcpThread::handleTcpThread(QObject *parent) : AbstractThread(parent)
{
    qDebug() << "创建一个handleTcpThread";
}

handleTcpThread::~handleTcpThread()
{
    QThread::sleep(1);                 // 阻塞1秒，模拟线程没有离开终止
    qDebug() << "~handleTcpThread()";
}

/**
 * @brief       处理传入的数据
 * @param arr
 */
void handleTcpThread::handleData(QByteArray arr)
{
    qDebug() << QString("这里是传入的数据长度：%1").arg(arr.count());

    emit outputData(arr);       // 将处理后的数据传递处理
}
