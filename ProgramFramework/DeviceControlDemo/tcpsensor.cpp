#include "tcpsensor.h"
#include <QDebug>
#include <QTcpSocket>
#include <handletcpthread.h>

TcpSensor::TcpSensor(QObject *parent) : AbstractSensor(parent)
{
    m_type = TcpType;

    m_tcpClient = new QTcpSocket(this);
    connect(m_tcpClient, &QTcpSocket::connected, this, &TcpSensor::on_connected);
    connect(m_tcpClient, &QTcpSocket::disconnected, this, &TcpSensor::on_disconnected);
    connect(m_tcpClient, &QTcpSocket::readyRead, this, &TcpSensor::on_readyRead);

    m_handleThread = new handleTcpThread;
    connect(m_handleThread, &AbstractThread::outputData, this, &TcpSensor::outputData);    // 将线程中的数据传递给UI

    qDebug() << "创建一个TcpSensor";
}

TcpSensor::~TcpSensor()
{
    qDebug() << "~TcpSensor()";
}

/**
 * @brief           传入Tcp 连接所需的IP和端口
 * @param hostName
 * @param port
 */
void TcpSensor::setTarget(const QString &hostName, quint16 port)
{
    m_ip = hostName;
    m_port = port;
}

/**
 * @brief 开始连接Tcp服务器
 */
void TcpSensor::open()
{
    if(!m_tcpClient) return;

    if(m_tcpClient->state() == QAbstractSocket::UnconnectedState)         // 判断连接状态
    {
        m_tcpClient->connectToHost(m_ip, m_port);      // 发起连接
    }
}

/**
 * @brief TCP断开连接
 */
void TcpSensor::close()
{
    if(!m_tcpClient) return;

    if(m_tcpClient->state() != QAbstractSocket::UnconnectedState)         // 判断连接状态
    {
        m_tcpClient->abort();                          // 断开连接
    }
}

/**
 * @brief      发送数据
 * @param arr
 */
void TcpSensor::write(const QByteArray &arr)
{
    qint64 len = m_tcpClient->write(arr);
    if(len < 0)
    {
        qWarning() << "发送失败！";
    }
    else if(len < arr.count())
    {
        qWarning() << "发送数据不完整！";
    }
    else
    {
        qInfo() << QString("发送数据长度：%1").arg(len);
    }
}

/**
 * @brief TCP连接成功
 */
void TcpSensor::on_connected()
{
    m_open = true;
    emit openState(true);
}

/**
 * @brief TCP断开连接
 */
void TcpSensor::on_disconnected()
{
    m_open = false;
    emit openState(false);
}

/**
 * @brief TCP接收数据
 */
void TcpSensor::on_readyRead()
{
    QByteArray arr = m_tcpClient->readAll();
    m_handleThread->inputData(arr);         // 读取Tcp数据并传入线程处理
}
