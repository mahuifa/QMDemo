#ifndef WIDGET_H
#define WIDGET_H

#include <QList>
#include <QWidget>
#include "tcpclient.h"
#include "tcpserver.h"

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

private:
    Ui::Widget *ui;
    QList<TCPClient*> m_tcpClients;   // 可打开任意多个TCP客户端
    QList<TCPServer*> m_tcpServers;   // 可打开任意多个TCP服务端
};
#endif // WIDGET_H
