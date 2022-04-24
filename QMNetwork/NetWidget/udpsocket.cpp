#include "udpsocket.h"
#include "ui_udpsocket.h"

#include <QNetworkDatagram>

UdpSocket::UdpSocket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpSocket)
{
    ui->setupUi(this);

    init();
    connectSlots();
}

UdpSocket::~UdpSocket()
{
    delete ui;
}

void UdpSocket::init()
{
    m_udpSocket = new QUdpSocket(this);
    ui->line_localAddress->setText("127.0.0.1");
}

void UdpSocket::connectSlots()
{
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpSocket::on_readyRead);

#if TEST1
    connect(m_udpSocket, &QUdpSocket::connected, this, &UdpSocket::on_connected);
#endif
}

/**
 * @brief       切换通信模式
 * @param index
 */
void UdpSocket::on_com_type_activated(int index)
{
    if(1 == index)
    {
        ui->line_group->setEnabled(true);
    }
    else
    {
        ui->line_group->setEnabled(false);
    }
}

/**
 * @brief 开始通信
 */
void UdpSocket::on_but_connect_clicked()
{
    if(ui->but_connect->text() == "打开")
    {
//            m_udpSocket->bind(ui->line_localAddress->text(), ui->spin_localPort->value());         // 绑定本地地址和端口号
//            m_udpSocket->bind(QHostAddress::AnyIPv4, 8888);
            bool ret = m_udpSocket->bind(ui->spin_localPort->value());         // 绑定本地地址和端口号
            if(ret)
            {
                qInfo() << "绑定本地地址成功！";
                ui->but_connect->setText("关闭");
#if TEST1
                m_udpSocket->connectToHost("127.0.0.1", ui->spin_peerPort->value());        // 使用和TCP类似的方法目标绑定IP、端口，在发送的时候就不需要指定IP、端口了
#endif
            }
            else
            {
                qWarning() << "绑定本地地址失败！";
            }
    }
    else
    {
        m_udpSocket->close();
        ui->but_connect->setText("打开");
    }
}

/**
 * @brief 接收数据
 */
void UdpSocket::on_readyRead()
{
    while (m_udpSocket->hasPendingDatagrams())                       // 判断是否有可读数据
    {
         QNetworkDatagram datagram = m_udpSocket->receiveDatagram(); // 读取数据
         QByteArray replyData = datagram.data();

         if(replyData.count())
         {
             QString strIP = datagram.senderAddress().toString().remove("::ffff:");
             int port = datagram.senderPort();

             if(ui->check_peerPort->isChecked())
             {
                 ui->line_peerAddress->setText(strIP);
                 ui->spin_peerPort->setValue(port);
             }
             ui->spin_recv->setValue(ui->spin_recv->value() + replyData.count());  // 统计接收的数据总大小
             if(ui->check_hexRecv->isChecked())
             {
                 ui->text_recv->append(QString("[%1 %2] ").arg(strIP).arg(port) + replyData.toHex(' ') );
             }
             else
             {
                 ui->text_recv->append(QString("[%1 %2] ").arg(strIP).arg(port)  + replyData);
             }
         }
    }
}


#if TEST1
void UdpSocket::on_connected()
{
    qDebug() << "连接成功！";
}
#endif

/**
 * @brief 发送数据
 */
void UdpSocket::on_but_send_clicked()
{
//    m_udpSocket->writeDatagram()
    QString str = ui->text_send->toPlainText();
#if 0
    QByteArray arr = str.toLocal8Bit();              // 根据编译器编码或者 QTextCodec::setCodecForLocale(codec);指定的编码方式将QString转换为QBytearray，一般为utf-8或者GBK，支持中文
#else
    QByteArray arr = str.toUtf8();                   // 指定以utf-8编码
#endif
    if(ui->check_hexSend->isChecked())               // 如果是16进制显示则转换
    {
        arr = QByteArray::fromHex(arr);
    }

#if TEST1
    qint64 len = m_udpSocket->write(arr);
#else
    qint64 len = m_udpSocket->writeDatagram(arr, QHostAddress(ui->line_peerAddress->text()), ui->spin_peerPort->value());
#endif
    if(len < 0)
    {
        qWarning() <<"发送失败！";
    }
    ui->spin_send->setValue(ui->spin_send->value() + len);
}



/**
 * @brief 清空接收区
 */
void UdpSocket::on_but_clearRecv_clicked()
{
    ui->text_recv->clear();
    ui->spin_recv->setValue(0);
}

/**
 * @brief 清空发送区
 */
void UdpSocket::on_but_clearSend_clicked()
{
    ui->text_send->clear();
    ui->spin_send->setValue(0);
}

/**
 * @brief          将接收区内容转换为十六进制显示
 * @param checked
 */
void UdpSocket::on_check_hexRecv_clicked(bool checked)
{
    QString value;
    if(checked)
    {
        value = ui->text_recv->toPlainText().toUtf8().toHex(' ');             // 01 02 03 1a 格式显示
    }
    else
    {
        value = QByteArray::fromHex(ui->text_recv->toPlainText().toUtf8());   // 123abc格式显示
    }
    ui->text_recv->setText(value);
}

/**
 * @brief           将发送区内容转换为十六进制显示
 * @param checked
 */
void UdpSocket::on_check_hexSend_clicked(bool checked)
{
    QString value;
    if(checked)
    {
        value = ui->text_send->toPlainText().toUtf8().toHex(' ');
    }
    else
    {
        value = QByteArray::fromHex(ui->text_send->toPlainText().toUtf8());
    }
    ui->text_send->setText(value);
}

