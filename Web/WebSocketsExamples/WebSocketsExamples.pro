TEMPLATE = subdirs

SUBDIRS += ChatClient \          # 使用QWebSocket实现的简单客户端程序
    ChatClientHtml
SUBDIRS += ChatServer          # 使用QWebSocketServer实现的简单服务端程序

