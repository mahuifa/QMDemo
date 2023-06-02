#include "iothread.h"

IOThread::IOThread(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_thread->start();   // 创建子线程

    connect(m_thread, &QThread::started, this, &IOThread::on_started);
    connect(m_thread, &QThread::finished, this, &IOThread::on_finished);
    connect(this, &IOThread::connectTcp, this, &IOThread::on_connectTcp);
    connect(this, &IOThread::sendData, this, &IOThread::on_sendData);
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
    connect(m_client, &QTcpSocket::connected, this, &IOThread::on_connected);
    connect(m_client, &QTcpSocket::disconnected, this, &IOThread::on_disconnected);
    connect(m_client, &QTcpSocket::readyRead, this, &IOThread::on_readyRead);
    connect(this, &IOThread::close, m_client, &QTcpSocket::close);
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

/**
 * @brief 接收数据
 */
void IOThread::on_readyRead()
{
    QByteArray buf = m_client->readAll();
    emit this->readyRead(buf);
}

void IOThread::on_connected()
{
    qDebug() << "连接成功";
    emit readyRead("连接成功");
}

void IOThread::on_disconnected()
{
    qDebug() << "关闭连接";
    emit readyRead("关闭连接");
}

/**
 * @brief      发起tcp连接
 * @param ip
 * @param port
 */
void IOThread::on_connectTcp(QString ip, ushort port)
{
    m_client->connectToHost(ip, port);
}

void IOThread::on_sendData(QByteArray data)
{
    qint64 ret = m_client->write(data);
    if(ret != data.count())
    {
        qWarning() << "发送数据错误";
    }
}
