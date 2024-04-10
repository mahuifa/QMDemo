#include "simpleudpgroup.h"
#include "ui_simpleudpgroup.h"

#include <qnetworkdatagram.h>
#include <QNetworkInterface>

SimpleUdpGroup::SimpleUdpGroup(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SimpleUdpGroup)
{
    ui->setupUi(this);

    this->setWindowTitle("简易版UDP组播通信Demo");

    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, &QUdpSocket::readyRead, this,
            &SimpleUdpGroup::on_readyRead);   // 当有可读数据时发出readyRead信号
}

SimpleUdpGroup::~SimpleUdpGroup()
{
    delete ui;
}

/**
 * @brief 读取UDP数据报
 */
void SimpleUdpGroup::on_readyRead()
{
    QNetworkDatagram datagram = m_udpSocket->receiveDatagram();   // 读取数据，这里需要添加qnetworkdatagram.h头文件
    ui->text_recv->append(datagram.data());                       // 显示读取到的数据
}

/**
 * @brief 开始绑定绑定IP端口，用于接收数据，并加入组播组
 */
void SimpleUdpGroup::on_but_connect_clicked()
{
    if (m_udpSocket->state() != QAbstractSocket::BoundState)   // 判断是否绑定绑定端口
    {
        bool ret = m_udpSocket->bind(
            QHostAddress::AnyIPv4,   // 使用组播时这里必须要指定AnyIPv4，不指定将会默认Any，加入组播会失败
            ui->spin_groupPort->value(),   // 绑定绑定接收数据的端口，一般用组播端口
            QUdpSocket::ShareAddress
                | QUdpSocket::ReuseAddressHint);   // ShareAddress允许一台电脑上多个程序绑定同一个端口，便于测试，但是在windows下需要加上ReuseAddressHint才有效
        if (ret)
        {
            for (auto face : QNetworkInterface::allInterfaces())
            {
                qDebug() << face.allAddresses() << face.name();
            }
            qInfo() << "绑定本地地址成功！";
            ret = m_udpSocket->joinMulticastGroup(QHostAddress(ui->line_groupAddress->text()));   // 加入组播组
            if (ret)
            {
                qInfo() << "加入组播组成功！";
                ui->but_connect->setText("关闭");
            }
            else
            {
                qWarning() << "加入组播组失败！";
            }
        }
        else
        {
            qWarning() << "绑定本地地址失败！";
        }
    }
    else
    {
        bool ret = m_udpSocket->leaveMulticastGroup(QHostAddress(ui->line_groupAddress->text()));
        if (ret)
        {
            qInfo() << "移除组播组成功！";
            m_udpSocket->abort();
            ui->but_connect->setText("打开");
        }
        else
        {
            qWarning() << "移除组播组失败！";
        }
    }
}

/**
 * @brief 发送数据，由于已经绑定了目标IP和目标端口号，在发送数据时就不用指定IP、端口了
 */
void SimpleUdpGroup::on_but_send_clicked()
{
    QString str = ui->text_send->toPlainText();
    qint64 len = m_udpSocket->writeDatagram(str.toUtf8(), QHostAddress(ui->line_groupAddress->text()),
                                            ui->spin_groupPort->value());
    qInfo() << QString("发送数据长度：%1").arg(len);
}
