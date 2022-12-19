#ifndef WEBSOCKETTRANSPORT_H
#define WEBSOCKETTRANSPORT_H

#include <QWebChannelAbstractTransport>
#include <qwebsocketserver.h>

class QWebSocket;

/**
 * 通过继承QWebChannelAbstractTransport实现QWebChannel交互功能
 * 使用QWebSocket作为通信接口
 */
class WebSocketTransport : public QWebChannelAbstractTransport
{
    Q_OBJECT
public:
    explicit WebSocketTransport(QWebSocket *client = nullptr);
    ~WebSocketTransport() override;

public slots:
    void sendMessage(const QJsonObject &message) override;
    void on_textMessageReceived(const QString &message);

private:
    QWebSocket* m_client = nullptr;
};

/**
 * WebSocket服务端程序，用于监听webSocket客户端连接，将连接的客户端使用
 * WebSocketTransport包装后传递出去
 */
class WebSocketServer : public QWebSocketServer
{
    Q_OBJECT
public:
    explicit WebSocketServer(QObject *parent = nullptr);

signals:
    void newWebSocketTransport(WebSocketTransport* transport);

private slots:
    void on_newConnection();
};

#endif // WEBSOCKETTRANSPORT_H
