﻿#ifndef WIDGET_H
#define WIDGET_H

#include <QList>
#include <QWidget>
#include "tcpclient.h"
#include "tcpserver.h"
#include "udpsocket.h"
#include "simpletcpclient.h"
#include "simpletcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_but_TCPClient_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_but_simpleTcpClient_clicked();

    void on_but_simpleTcpServer_clicked();

private:
    Ui::Widget *ui;
    /******************简易版网络通信Demo********************/
    QList<SimpleTcpClient*> m_simpleTcpClients;   // 可打开任意多个TCP客户端
    QList<SimpleTcpServer*> m_simpleTcpServers;   // 可打开任意多个TCP服务端


    /******************较复杂版网络通信Demo********************/
    QList<TCPClient*> m_tcpClients;   // 可打开任意多个TCP客户端
    QList<TCPServer*> m_tcpServers;   // 可打开任意多个TCP服务端
    QList<UdpSocket*> m_udpsockets;   // 可打开任意多个UDP通信创建
};
#endif // WIDGET_H
