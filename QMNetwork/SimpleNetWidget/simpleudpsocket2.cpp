#include "simpleudpsocket2.h"
#include "ui_simpleudpsocket2.h"

#include <qnetworkdatagram.h>

SimpleUdpSocket2::SimpleUdpSocket2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleUdpSocket2)
{
    ui->setupUi(this);

    this->setWindowTitle("简易版类TCP方式UDP单播通信Demo");

    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &SimpleUdpSocket2::on_readyRead);         // 当有可读数据时发出readyRead信号
    connect(m_udpSocket, &QUdpSocket::connected, this, &SimpleUdpSocket2::on_connected);         // 绑定目标IP、端口成功后发出connected信号
}

SimpleUdpSocket2::~SimpleUdpSocket2()
{
    delete ui;
}

void SimpleUdpSocket2::on_readyRead()
{
    ui->text_recv->append(m_udpSocket->readAll());                      // 显示读取到的数据
}

/**
 * @brief 连接IP地址、端口号成功，修改按键状态
 */
void SimpleUdpSocket2::on_connected()
{
    ui->but_connect->setText("关闭");
    ui->line_peerAddress->setEnabled(false);        // 禁用目标IP、端口输入框，防止修改
    ui->spin_localPort->setEnabled(false);
    ui->spin_localPort->setEnabled(false);
}

/**
 * @brief 开始绑定绑定IP端口，用于接收数据；
 *        使用类似TCP的connectToHost绑定目标IP、目标端口号，在发送数据时不用指定ip和端口了
 */
void SimpleUdpSocket2::on_but_connect_clicked()
{
    if(m_udpSocket->state() != QAbstractSocket::ConnectedState)            // 如果未绑定本地ip、端口(使用connectToHost可让状态达到ConnectedState)
    {
        bool ret = m_udpSocket->bind(ui->spin_localPort->value());         // 绑定本地地址和端口号
        if(ret)
        {
            qInfo() << "绑定本地地址成功！";
            m_udpSocket->connectToHost(QHostAddress(ui->line_peerAddress->text()), ui->spin_peerPort->value());   // 使用类似TCP连接目标IP、端口的方式绑定目标IP、端口
        }
        else
        {
            qWarning() << "绑定本地地址失败！";
        }
    }
    else
    {
        m_udpSocket->abort();
        ui->but_connect->setText("打开");
        ui->line_peerAddress->setEnabled(true);        // 结束通信后允许修改
        ui->spin_localPort->setEnabled(true);
        ui->spin_localPort->setEnabled(true);
    }
}

/**
 * @brief 发送数据，由于已经绑定了目标IP和目标端口号，在发送数据时就不用指定IP、端口了
 */
void SimpleUdpSocket2::on_but_send_clicked()
{
    QString str = ui->text_send->toPlainText();
    qint64 len = m_udpSocket->write(str.toUtf8());
    qInfo() << QString("发送数据长度：%1").arg(len);
}

