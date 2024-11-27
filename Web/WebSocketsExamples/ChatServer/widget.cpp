#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QWebSocketServer服务端简单示例 - V%1").arg(APP_VERSION));  // 设置窗口标题

    m_server = new QWebSocketServer("web服务端", QWebSocketServer::NonSecureMode, this);
    connect(m_server, &QWebSocketServer::newConnection, this, &Widget::on_newConnection);
    connect(m_server, &QWebSocketServer::closed, this, &Widget::on_closed);
}

Widget::~Widget()
{
    // 关闭服务端
    if(m_server->isListening())
    {
        m_server->close();
    }
    // 关闭所有客户端
    qDeleteAll(m_clients.begin(), m_clients.end());

    delete ui;
}

/**
 * @brief 有新的客户端发起连接
 */
void Widget::on_newConnection()
{
    QWebSocket* client = m_server->nextPendingConnection();   // 获取连接成功的客户端

    connect(client, &QWebSocket::textMessageReceived, this, &Widget::on_textMessageReceived);
    connect(client, &QWebSocket::disconnected, this, &Widget::on_disconnected);

    // 将所有客户端加入列表
    m_clients << client;
    ui->textEdit->append(QString("[%1:%2] 连接成功！").arg(client->peerAddress().toString()).arg(client->peerPort()));
}

/**
 * @brief 服务端关闭监听，关闭后不再接收新的客户端的连接请求
 */
void Widget::on_closed()
{
    ui->textEdit->append("服务端关闭监听！");
    ui->but_listen->setText("开启监听");
}

/**
 * @brief 开始监听/关闭服务端监听
 */
void Widget::on_but_listen_clicked()
{
    if(!m_server->isListening())
    {
        bool ret = m_server->listen(QHostAddress::AnyIPv4, 1234);
        if(ret)
        {
            ui->textEdit->append(QString("开始监听：%1").arg(m_server->serverUrl().toString()));
            ui->but_listen->setText("停止监听");
        }
    }
    else
    {
        m_server->close();
    }
}

/**
 * @brief 向所有连接的客户端发送数据
 */
void Widget::on_but_send_clicked()
{
    for(auto client : m_clients)
    {
        client->sendTextMessage(ui->lineEdit_send->text());
    }
}

/**
 * @brief          接收信息并将信息转发给所有客户端
 * @param message
 */
void Widget::on_textMessageReceived(const QString &message)
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    for(auto client : m_clients)
    {
        if(client != socket)   // 向所有连接的客户端转发信息，除了当前信息的发出者
        {
            client->sendTextMessage(message);
        }
    }
    ui->textEdit->append(QString("[%1:%2] %3").arg(socket->peerAddress().toString()).arg(socket->peerPort()).arg(message));
}

/**
 * @brief 断开连接时移除对应的客户端
 */
void Widget::on_disconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if(socket)
    {
        m_clients.removeAll(socket);
        socket->deleteLater();
    }
    ui->textEdit->append(QString("[%1:%2] 断开连接！").arg(socket->peerAddress().toString()).arg(socket->peerPort()));
}
