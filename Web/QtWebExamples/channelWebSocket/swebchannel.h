/******************************************************************************
 * @文件名     swebchannel.h
 * @功能       通过继承QWebChannel，将创建管理WebSocket服务端功能，使用connectTo设置通信接口功能
 *            封装在类内部，简化外部操作连接，一个SWebChannel对应一个WebSocket服务端、多个客户端
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/12/19
 * @备注
 *****************************************************************************/
#ifndef SWEBCHANNEL_H
#define SWEBCHANNEL_H

#include <QWebChannel>
#include "websockettransport.h"

class SWebChannel : public QWebChannel
{
    Q_OBJECT
public:
    explicit SWebChannel(QObject *parent = nullptr);


    bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

signals:

private slots:

private:
    WebSocketServer m_server;
    QHostAddress    m_address;
    quint16         m_port;
};

#endif // SWEBCHANNEL_H
