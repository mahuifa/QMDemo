#include "simpleudpsocket1.h"
#include "ui_simpleudpsocket1.h"

#include <qnetworkdatagram.h>

SimpleUdpSocket1::SimpleUdpSocket1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleUdpSocket1)
{
    ui->setupUi(this);

    this->setWindowTitle("简易版传统方式UDP单播通信Demo");

    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &SimpleUdpSocket1::on_readyRead);     // 当有可读数据时发出readyRead信号
}

SimpleUdpSocket1::~SimpleUdpSocket1()
{
    delete ui;
}

/**
 * @brief 读取UDP数据报
 */
void SimpleUdpSocket1::on_readyRead()
{
    QNetworkDatagram datagram = m_udpSocket->receiveDatagram();  // 读取数据，这里需要添加qnetworkdatagram.h头文件
    ui->text_recv->append(datagram.data());                      // 显示读取到的数据
}

/**
 * @brief 开始绑定绑定IP端口，用于接收数据
 */
void SimpleUdpSocket1::on_but_connect_clicked()
{
    if(m_udpSocket->state() != QAbstractSocket::BoundState)                // 如果未绑定本地ip、端口（第一种方式udp状态只到BoundState）
    {
        bool ret = m_udpSocket->bind(ui->spin_localPort->value());         // 绑定本地地址和端口号
        if(ret)
        {
            qInfo() << "绑定本地地址成功！";
            ui->but_connect->setText("关闭");
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
    }
}

/**
 * @brief 发送数据，只要有目标IP、端口就可以发送，与是否绑定本地IP端口无关
 *        如果发送时未绑定本地端口号，则默认分配一个，并将udp状态置为BoundState
 */
void SimpleUdpSocket1::on_but_send_clicked()
{
    QString str = ui->text_send->toPlainText();
    qint64 len = m_udpSocket->writeDatagram(str.toUtf8(), QHostAddress(ui->line_peerAddress->text()), ui->spin_peerPort->value());
    qInfo() << QString("发送数据长度：%1").arg(len);
}

