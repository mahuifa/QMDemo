#include "simpletcpserver.h"
#include "ui_simpletcpserver.h"

#include <qtcpsocket.h>

SimpleTcpServer::SimpleTcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleTcpServer)
{
    ui->setupUi(this);

    this->setWindowTitle("简易版本TCP 服务端Demo");

    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &SimpleTcpServer::on_newConnection);    // 监听新的客户端连接
}

SimpleTcpServer::~SimpleTcpServer()
{
    if(m_server->isListening())          // 判断是否正在监听连接
    {
        if(m_client) m_client->abort();       // 判断是否为空再释放
        m_server->close();
    }
    delete ui;
}


void SimpleTcpServer::on_but_connect_clicked()
{
    if(!m_server->isListening())          // 判断是否正在监听连接
    {
        // 告诉服务器侦听地址和端口上的传入连接。如果端口为0，则会自动选择一个端口。
        // 如果地址是QHostAddress:：Any，服务器将监听所有网络接口。
        bool ret = m_server->listen(QHostAddress::Any, ui->spin_localPort->value());
        if(ret)
        {
            ui->but_connect->setText("停止");
        }
        else
        {
            qDebug() << QString("tcpServer绑定监听IP、端口失败：%1 %2").arg(m_server->errorString()).arg(m_server->serverError());
        }
    }
    else
    {
        if(m_client) m_client->abort();       // 判断是否为空再释放
        m_server->close();
        ui->but_connect->setText("开始监听");
    }
}

/**
 * @brief 如果有新的TCP客户端发起连接，则会在这个函数中返回连接成功的QTcpSocket；
 *        这里只保留第一个连接成功的客户端，其它的自动断开连接
 */
void SimpleTcpServer::on_newConnection()
{
    QTcpSocket* tcpSocket = m_server->nextPendingConnection();    // 存在内存泄漏，最好使用时通过hasPendingConnections判断是否有未返回的连接
    if(tcpSocket)
    {
        if(m_client)         // 如果已经有连接的 客户端，则断开后面所有的连接
        {
            tcpSocket->deleteLater();
            return;
        }
        m_client = tcpSocket;
        connect(tcpSocket, &QTcpSocket::disconnected, this, &SimpleTcpServer::on_disconnected);        // 断开连接
        connect(tcpSocket, &QTcpSocket::readyRead, this, &SimpleTcpServer::on_readyRead);              // 接收客户端发来的数据
    }
}

/**
 * @brief 断开连接
 */
void SimpleTcpServer::on_disconnected()
{
    disconnect(m_client, &QTcpSocket::disconnected, this, &SimpleTcpServer::on_disconnected);          // 断开绑定的信号槽
    disconnect(m_client, &QTcpSocket::readyRead, this, &SimpleTcpServer::on_readyRead);
    //delete m_client;                // 这里不能用delete，否则在vs里会报错 0x00007FFBF870A1CE (Qt5Networkd.dll)处(位于 QMNetwork.exe 中)引发的异常: 0xC0000005: 读取位置 0xFFFFFFFFFFFFFFFF 时发生访问冲突。
    m_client->deleteLater();        // 移除已经断开连接的Client（注意这里不能使用delete，否则在vs中会报错），但这种方式不会置为 空
    m_client = nullptr;              
}

/**
 * @brief 接收数据，这里和TCP Client是一样的
 */
void SimpleTcpServer::on_readyRead()
{
    QByteArray dataArr = m_client->readAll();        // 一次性读取缓存中所有数据
    if(dataArr.count() <= 0)   // 没有读取到数据
    {
        return;
    }

    ui->text_recv->append(dataArr);        // 显示读取到的数据
}

/**
 * @brief 发送数据，这里和TCP Client是一样的
 */
void SimpleTcpServer::on_but_send_clicked()
{
    if(!m_client) return;                                                       // 判断是否为空，如果为空则未连接
    if(m_client->state() != QAbstractSocket::ConnectedState) return;  // 判断TCP是否已连接，如果没有连接则不发送数据

    QString str = ui->text_send->toPlainText();         // 获取输入框的内容
    qint64 len = m_client->write(str.toUtf8());      // 发送数据，toUtf8()是为了支持发送中文，但是如果接收端编码方式不同则中文可能显示乱码
    if(len < 0)
    {
        qWarning() <<"发送失败！";
    }
}

