#include "tcpserver.h"
#include "ui_tcpserver.h"
#include <QAbstractSocket>
#include <QDebug>
#include <QListWidgetItem>
#include <qhostinfo.h>


TCPServer::TCPServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCPServer)
{
    ui->setupUi(this);
    this->setWindowTitle("TCP服务端Demo");

    init();
    connectSlots();
}

TCPServer::~TCPServer()
{
#ifdef QT_DEBUG
    qDebug() <<"~TCPServer()";
#endif
    delete ui;
}


void TCPServer::init()
{
    m_tcpServer = new QTcpServer(this);
    m_tcpServer->setMaxPendingConnections(30);     // 设置最大允许连接数，不设置的话默认为30(如果设置过大就需要考虑内存泄漏问题)
    ui->line_localAddress->setText("127.0.0.1");
}

void TCPServer::connectSlots()
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &TCPServer::on_newConnection);    // 监听新的客户端连接
    connect(m_tcpServer, &QTcpServer::acceptError, this, &TCPServer::on_acceptError);        // 当接受新连接导致错误时，会发出此信号
}


void TCPServer::on_newConnection()
{
    while (m_tcpServer->hasPendingConnections())
    {
        QTcpSocket* tcpSocket = m_tcpServer->nextPendingConnection();      // 存在内存泄漏，最好使用时通过hasPendingConnections判断是否有未返回的连接
        if(tcpSocket)
        {
            m_tcpClients.append(tcpSocket);
        }
        connect(tcpSocket, &QTcpSocket::disconnected, this, &TCPServer::on_disconnected);        // 断开连接
        connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPServer::on_readyRead);

        QString strPeer = QString("%1 %2").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort());
        strPeer.remove("::ffff:");
        addPeer(strPeer);
    }
}

/**
 * @brief               处理连接中的异常详细
 * @param socketError
 */
void TCPServer::on_acceptError(QAbstractSocket::SocketError socketError)
{
    qWarning() << QString("TcpServer异常：%1").arg(socketError);
}

/**
 * @brief       添加已连接的目标信息
 * @param peer  IP + 端口
 */
void TCPServer::addPeer(const QString &peer)
{
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    QCheckBox* checkbox = new QCheckBox();          // 添加复选框
    checkbox->setText(peer);
    //设置item的高度
    item->setSizeHint(QSize(0,20));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, checkbox);
}

/**
 * @brief 开始监听连接
 */
void TCPServer::on_but_connect_clicked()
{
    if(!m_tcpServer) return;

    if(!m_tcpServer->isListening())          // 判断是否正在监听连接
    {
        // 告诉服务器侦听地址和端口上的传入连接。如果端口为0，则会自动选择一个端口。
        // 如果地址是QHostAddress:：Any，服务器将监听所有网络接口。
        bool ret = m_tcpServer->listen(QHostAddress::Any, ui->spin_localPort->value());
        if(ret)
        {
            ui->but_connect->setText("停止");
        }
        else
        {
            qDebug() << QString("tcpServer绑定监听IP、端口失败：%1 %2").arg(m_tcpServer->errorString()).arg(m_tcpServer->serverError());
        }
    }
    else
    {
        m_tcpServer->close();
        ui->but_connect->setText("开始监听");
        for(int i = m_tcpClients.count() - 1; i >= 0 ; i--)          // 关闭所有连接的TCP Client
        {
            m_tcpClients.at(i)->abort();
        }
    }
}

/**
 * @brief Socket  已断开连接
 */
void TCPServer::on_disconnected()
{
    for(int i = 0; i < m_tcpClients.count(); i++)
    {
        if(m_tcpClients.at(i)->state() != QAbstractSocket::ConnectedState)       // 未连接
        {
            disconnect(m_tcpClients.at(i), &QTcpSocket::disconnected, this, &TCPServer::on_disconnected);
            disconnect(m_tcpClients.at(i), &QTcpSocket::readyRead, this, &TCPServer::on_readyRead);
            m_tcpClients.takeAt(i)->deleteLater();       // 移除已经断开连接的Client（注意这里不能使用delete，否则在vs中会报错）
            QListWidgetItem * item = ui->listWidget->item(i);
            ui->listWidget->removeItemWidget(item);
            delete item;                    // 不加这一行 listwidget的count不会变化
            item = nullptr;
        }
    }
}

/**
 * @brief 接收数据
 */
void TCPServer::on_readyRead()
{
    for(int i = 0; i < ui->listWidget->count(); i++)
    {
        if(m_tcpClients.at(i)->bytesAvailable() <= 0) continue;;       // 如果无可读数据则返回

        QByteArray arr = m_tcpClients.at(i)->readAll();                // 读取数据
        QCheckBox* checkBox = (QCheckBox*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        if(checkBox->isChecked())                                      // 勾选的已连接Client则显示接收的信息
        {
            if(arr.count() <= 0)
            {
                continue;
            }
            ui->spin_recv->setValue(ui->spin_recv->value() + arr.count());  // 统计接收的数据总大小
            QString strPeer = checkBox->text();
            if(ui->check_hexRecv->isChecked())
            {
                ui->text_recv->append(QString("[%1] ").arg(strPeer) + arr.toHex(' '));
            }
            else
            {
                ui->text_recv->append(QString("[%1] ").arg(strPeer) + arr);
            }
        }
    }
}

/**
 * @brief 清空接收区
 */
void TCPServer::on_but_clearRecv_clicked()
{
    ui->text_recv->clear();
    ui->spin_recv->setValue(0);
}

/**
 * @brief 清空发送区
 */
void TCPServer::on_but_clearSend_clicked()
{
    ui->text_send->clear();
    ui->spin_send->setValue(0);
}

/**
 * @brief          将接收区内容转换为十六进制显示
 * @param checked
 */
void TCPServer::on_check_hexRecv_clicked(bool checked)
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
void TCPServer::on_check_hexSend_clicked(bool checked)
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

/**
 * @brief  发送数据
 */
void TCPServer::on_but_send_clicked()
{
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

    qint64 len = sendData(arr);
    if(len < 0)
    {
        qWarning() <<"发送失败！";
    }
    ui->spin_send->setValue(ui->spin_send->value() + len);
}

/**
 * @brief       向所有已连接并且勾选的Client发送数据
 * @param data
 * @return      返回发送数据的最大长度
 */
qint64 TCPServer::sendData(const QByteArray &data)
{
    qint64 len = 0;
    for(int i = 0; i < ui->listWidget->count(); i++)
    {
        QCheckBox* checkBox = (QCheckBox*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        if(checkBox->isChecked())                                      // 勾选的已连接Client则显示接收的信息
        {
             qint64 ret = m_tcpClients.at(i)->write(data);
             if(ret > len)
             {
                 len = ret;
             }
        }
    }
    return len;
}
