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
