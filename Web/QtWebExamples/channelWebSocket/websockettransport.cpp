#include "websockettransport.h"

#include <qjsondocument.h>
#include <qwebsocket.h>
#include <QJsonObject>

WebSocketTransport::WebSocketTransport(QWebSocket *client)
    : QWebChannelAbstractTransport(client)
    , m_client(client)
{
    // 当WebSocket收到信息后，将信息处理后并传递给QWebChannelAbstractTransport
    connect(m_client, &QWebSocket::textMessageReceived, this, &WebSocketTransport::on_textMessageReceived);
    // 当WebSocket断开连接后，释放当前对象
    connect(m_client, &QWebSocket::disconnected, this, &WebSocketTransport::deleteLater);
    connect(m_client, &QWebSocket::disconnected, [=]()
    {
        qDebug() << "Web端断开连接！";
    });
}

WebSocketTransport::~WebSocketTransport()
{
    if(m_client)
    {
        m_client->deleteLater();
    }
}

/**
 * @brief           QWebChannelAbstractTransport中定义的接口，当使用QChannel发送信息时，数据传递到这里
 *                  在这个函数中处理需要发送的数据，并通过WebSocket发送出去
 * @param message
 */
void WebSocketTransport::sendMessage(const QJsonObject &message)
{
    QJsonDocument doc(message);
    m_client->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));  // 将需要发送的JSON数据转成字符串发送
}

/**
 * @brief          通过WebSocket接收到网页javascript发来的信息时，
 *                 在这个函数中将字符串数据转换为JSON数据传递给QWebChannel
 * @param message
 */
void WebSocketTransport::on_textMessageReceived(const QString &message)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &error);
    if(error.error)  // 字符串转JSON成功，error为0，失败则error > 0
    {
        qWarning() << QString("%1 无法将文本消息解析为JSON对象：%2").arg(error.errorString()).arg(message);
        return;
    }
    if(!doc.isObject())
    {
        qWarning() << QString("收到的JSON消息不是JSON对象：%1").arg(QString(doc.toJson()));
    }

    emit messageReceived(doc.object(), this);    // 将收到的信息发送给QWebChannel
}



WebSocketServer::WebSocketServer(QObject *parent)
    : QWebSocketServer("QChannel通信接口服务", NonSecureMode, parent)
{
    connect(this, &QWebSocketServer::newConnection, this, &WebSocketServer::on_newConnection);
}

/**
 * @brief 当有新的客户端连接时，将连接的客户端通过WebSocketTransport类包装后传递出去
 */
void WebSocketServer::on_newConnection()
{
    qDebug() << "有新的连接";
    emit newWebSocketTransport(new WebSocketTransport(this->nextPendingConnection()));
}
