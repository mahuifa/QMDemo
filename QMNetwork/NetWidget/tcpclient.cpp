#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QHostAddress>
#include <QDebug>
#include <QByteArray>
#include <QHostInfo>

TCPClient::TCPClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCPClient)
{
    ui->setupUi(this);
    this->setWindowTitle("TCP客户端Demo");

    init();
    connectSlots();
}

TCPClient::~TCPClient()
{
#ifdef QT_DEBUG
    qDebug() <<"~TCPClient()";
#endif

    if(m_tcpClient->state() != QAbstractSocket::UnconnectedState)         // 判断连接状态
    {
        m_tcpClient->abort();
    }
    delete ui;
}

void TCPClient::init()
{
    m_tcpClient = new QTcpSocket(this);
    ui->line_localAddress->setText("127.0.0.1");
}

void TCPClient::connectSlots()
{
    connect(m_tcpClient, &QTcpSocket::connected, this, &TCPClient::on_connected);
    connect(m_tcpClient, &QTcpSocket::disconnected, this, &TCPClient::on_disconnected);
    connect(m_tcpClient, &QTcpSocket::stateChanged, this, &TCPClient::on_stateChanged);
    connect(m_tcpClient, &QTcpSocket::readyRead, this, &TCPClient::on_readyRead);

#if (QT_VERSION <= QT_VERSION_CHECK(5,15,0))        // qt5.15 后error已经弃用，这里改用errorOccurred
    connect(m_tcpClient, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &TCPClient::on_errorOccurred);    // 由于QAbstractSocket中有两个error()，所以不能直接使用Qt5的信号槽绑定方式
#else
    connect(m_tcpClient, &QTcpSocket::errorOccurred, this, &TCPClient::on_errorOccurred);
#endif
}

/**
 * @brief 调用 connectToHost() 并成功建立连接。
 */
void TCPClient::on_connected()
{
    ui->but_connect->setText("断开连接");
    ui->spin_localPort->setValue(m_tcpClient->localPort());
    qInfo() << QString("成功连接：%1 %2").arg(m_tcpClient->peerName()).arg(m_tcpClient->peerPort());
}

/**
 * @brief Socket已断开连接
 */
void TCPClient::on_disconnected()
{
    ui->but_connect->setText("连接");
    qInfo() <<QString("断开连接：%1 %2").arg(m_tcpClient->peerName()).arg(m_tcpClient->peerPort());
}

/**
 * @brief       QAbstractSocket 的状态发生变化
 * @param state QAbstractSocket::SocketState 不是注册的元类型，因此对于Qt::QueuedConnection信号槽连接类型，
 *              必须使用 Q_DECLARE_METATYPE() 和 qRegisterMetaType() 注册它。
 *              QAbstractSocket::UnconnectedState	0	套接字未连接。
 *              QAbstractSocket::HostLookupState	1	套接字正在查询主机。
 *              QAbstractSocket::ConnectingState	2	套接字开始建立连接。
 *              QAbstractSocket::ConnectedState	    3	新的连接已建立。
 *              QAbstractSocket::BoundState	        4	套接字已绑定到一个地址和端口。
 *              QAbstractSocket::ClosingState	    6	套接字即将关闭（数据可能仍在等待写入）。
 *              QAbstractSocket::ListeningState	    5	套接字仅限内部使用。
 */
void TCPClient::on_stateChanged(QAbstractSocket::SocketState state)
{
    qInfo() << "状态改变："<< state;
}

/**
 * @brief        发生错误
 * @param error  错误异常信息，QAbstractSocket::SocketError 不是已注册的元类型，
 *               因此对于排队连接，您必须使用 Q_DECLARE_METATYPE() 和 qRegisterMetaType() 注册它。
 *               ConnectionRefusedError：连接被对等方拒绝（或超时）。
 *               RemoteHostClosedError：远程主机关闭了连接。请注意，客户端Socket将在发送远程关闭通知后关闭。
 *               HostNotFoundError：找不到主机地址。
 *               SocketAccessError：Socket操作失败，因为应用程序缺少所需的权限。
 *               SocketResourceError：本地系统资源不足（例如，Socket过多）。
 *               SocketTimeoutError：Socket操作超时。
 *               DatagramTooLargeError：数据报大于操作系统的限制。
 *               NetworkError：网络出现错误（例如，网现被拔出）。
 *               AddressInUseError：指定给 bind() 的地址已在使用中并设置为独占。
 *               SocketAddressNotAvailableError：指定给 bind() 的地址不属于主机。
 *               UnsupportedSocketOperationError：本地操作系统不支持请求的Socket操作（例如，缺乏 IPv6 支持）。
 *               ProxyAuthenticationRequiredError：Socket正在使用代理，并且代理需要身份验证。
 *               SslHandshakeFailedError：SSL/TLS 握手失败，因此连接被关闭（仅在 QSslSocket 中使用）
 *               UnfinishedSocketOperationError：上次尝试的操作尚未完成（仍在后台进行）。
 *               ProxyConnectionRefusedError：无法联系代理服务器，因为与该服务器的连接被拒绝
 *               ProxyConnectionClosedError：在与最终对等方的连接建立之前，与代理服务器的连接意外关闭
 *               ProxyConnectionTimeoutError：与代理服务器的连接超时或代理服务器在身份验证阶段停止响应。
 *               ProxyNotFoundError：未找到使用 setProxy()（或应用程序代理）设置的代理地址。
 *               ProxyProtocolError：与代理服务器的连接协商失败，因为无法理解来自代理服务器的响应。
 *               OperationError：当Socket处于不允许的状态时尝试进行操作。
 *               SslInternalError：正在使用的 SSL 库报告了一个内部错误。这可能是库安装错误或配置错误的结果。
 *               SslInvalidUserDataError：提供了无效数据（证书、密钥、密码等），其使用导致 SSL 库中出现错误。
 *               TemporaryError：发生临时错误（例如，操作会阻塞而Socket是非阻塞的）。
 *               UnknownSocketError：发生不明错误。
 */
void TCPClient::on_errorOccurred(QAbstractSocket::SocketError error)
{
    qWarning() << "出现异常：" <<error;
}

/**
 * @brief 接收数据
 */
void TCPClient::on_readyRead()
{
    QByteArray arr = m_tcpClient->readAll();

    if(arr.count() <= 0)
    {
        return;
    }
    ui->spin_recv->setValue(ui->spin_recv->value() + arr.count());
    if(ui->check_hexRecv->isChecked())
    {
        ui->text_recv->append(arr.toHex(' '));
    }
    else
    {
        ui->text_recv->append(arr);
    }
}

/**
 * @brief TCP连接
 */
void TCPClient::on_but_connect_clicked()
{
    if(!m_tcpClient) return;

    if(m_tcpClient->state() == QAbstractSocket::UnconnectedState)         // 判断连接状态
    {
        m_tcpClient->connectToHost(ui->line_peerAddress->text(), ui->spin_peerPort->value());
    }
    else
    {
#if 1   // 使用abort函数立即关闭套接字，丢弃写入缓冲区中的任何未决数据。
        m_tcpClient->abort();

#else   // 等待数据发送完成再关闭

#if 0
        m_tcpClient->disconnectFromHost();  // 不一定立即关闭，如果有待写入的数据等待写入，将进入 ClosingState状态 并等待所有数据写入完毕
#else
        m_tcpClient->close();               // 关闭 QIODevice并调用disconnectFromHost
#endif
        qDebug() <<"IO状态：" << m_tcpClient->isOpen();                 // 可通过这一行代码测试Close和disconnectFromHost的区别
        if (m_tcpClient->state() == QAbstractSocket::UnconnectedState) // 如果调用close后没有立即关闭则等待 1秒中
        {
            if(m_tcpClient->waitForDisconnected(1000))
            {
                qInfo() <<"关闭成功!";
            }
            else
            {
                qWarning() << "关闭失败："<<m_tcpClient->error();
            }
        }
#endif
    }
}

/**
 * @brief 发送数据
 */
void TCPClient::on_but_send_clicked()
{
    if(!m_tcpClient) return;
    if(m_tcpClient->state() != QAbstractSocket::ConnectedState) return;  // 判断是否已连接

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

    qint64 len = m_tcpClient->write(arr);
    if(len < 0)
    {
        qWarning() <<"发送失败！";
    }
    ui->spin_send->setValue(ui->spin_send->value() + len);
}

/**
 * @brief 清空接收区
 */
void TCPClient::on_but_clearRecv_clicked()
{
    ui->text_recv->clear();
    ui->spin_recv->setValue(0);
}

/**
 * @brief 清空发送区
 */
void TCPClient::on_but_clearSend_clicked()
{
    ui->text_send->clear();
    ui->spin_send->setValue(0);
}

/**
 * @brief           将发送区内容转换为十六进制显示
 * @param checked
 */
void TCPClient::on_check_hexSend_clicked(bool checked)
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
 * @brief          将接收区内容转换为十六进制显示
 * @param checked
 */
void TCPClient::on_check_hexRecv_clicked(bool checked)
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


