#include "swebchannel.h"

SWebChannel::SWebChannel(QObject *parent) : QWebChannel(parent)
{
    // 将连接成功的通道传递给QWebChannel
    connect(&m_server, &WebSocketServer::newWebSocketTransport, this, &SWebChannel::connectTo);
}

/**
 * @brief          设置WebSocket服务端监听
 * @param address  监听的地址
 * @param port     监听的端口号
 * @return
 */
bool SWebChannel::listen(const QHostAddress &address, quint16 port)
{
    // 如果新设置的监听地址、端口和已经监听的相同，则直接返回
    if(m_address == address && m_port == port && m_server.isListening())
    {
        return true;
    }
    m_address = address;
    m_port = port;
    if(m_server.isListening())   // 如果已经在监听了则关闭已有监听，重新监听新的地址、端口
    {
        m_server.close();
    }

    return m_server.listen(address, port);
}
