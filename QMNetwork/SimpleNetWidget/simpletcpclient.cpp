#include "simpletcpclient.h"
#include "ui_simpletcpclient.h"

#include <qhostaddress.h>

SimpleTcpClient::SimpleTcpClient(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SimpleTcpClient)
{
    ui->setupUi(this);

    this->setWindowTitle("简易版本TCP 客户端Demo");

    m_tcpClient = new QTcpSocket(this);
    connect(m_tcpClient, &QTcpSocket::connected, this, &SimpleTcpClient::on_connected);         // TCP连接成功后发出connected信号
    connect(m_tcpClient, &QTcpSocket::disconnected, this, &SimpleTcpClient::on_disconnected);   // TCP断开连接后发出disconnected信号
    connect(m_tcpClient, &QTcpSocket::readyRead, this, &SimpleTcpClient::on_readyRead);         // 当有可读数据时发出readyRead信号
}

SimpleTcpClient::~SimpleTcpClient()
{
    if (m_tcpClient->state() != QAbstractSocket::UnconnectedState)
    {
        m_tcpClient->abort();   // 因为再关闭时会修改but_connect文本，所以如果由系统再析构时自动释放则会出现的错误，再这里关闭了就不会出现了
    }
    delete ui;
}

/**
 * @brief TCP连接/断开控制，不需要指定本地端口，默认自动分配一个端口
 */
void SimpleTcpClient::on_but_connect_clicked()
{
    if (m_tcpClient->state() == QAbstractSocket::UnconnectedState)   // 判断连接状态，如果未连接则开始连接
    {
        m_tcpClient->connectToHost(ui->line_peerAddress->text(),   // 目标IP地址
                                   ui->spin_peerPort->value());    // 目标端口号
    }
    else   // 如果不是未连接则断开连接
    {
        m_tcpClient->abort();
    }
}

/**
 * @brief TCP连接成功，修改按键状态
 */
void SimpleTcpClient::on_connected()
{
    ui->but_connect->setText("断开连接");
}

/**
 * @brief TCP断开连接，修改按键状态
 */
void SimpleTcpClient::on_disconnected()
{
    ui->but_connect->setText("连接");
}

/**
 * @brief TCP读取数据并显示
 */
void SimpleTcpClient::on_readyRead()
{
    QByteArray dataArr = m_tcpClient->readAll();   // 一次性读取缓存中所有数据
    if (dataArr.count() <= 0)                      // 没有读取到数据
    {
        return;
    }

    ui->text_recv->append(dataArr);   // 显示读取到的数据
}

/**
 * @brief 发送数据
 */
void SimpleTcpClient::on_but_send_clicked()
{
    if (m_tcpClient->state() != QAbstractSocket::ConnectedState)
        return;   // 判断TCP是否已连接，如果没有连接则不发送数据

    QString str = ui->text_send->toPlainText();      // 获取输入框的内容
    qint64 len = m_tcpClient->write(str.toUtf8());   // 发送数据，toUtf8()是为了支持发送中文，但是如果接收端编码方式不同则中文可能显示乱码
    if (len < 0)
    {
        qWarning() << "发送失败！";
    }
}
