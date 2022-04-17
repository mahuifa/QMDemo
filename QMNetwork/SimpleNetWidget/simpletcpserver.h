/******************************************************************************
 * @文件名     simpletcpserver.h
 * @功能       简易的TCP服务端Demo，只保留接听TCP（1个）客户端连接/断开、 数据发送、接收功能
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/17
 * @备注
 *****************************************************************************/
#ifndef SIMPLETCPSERVER_H
#define SIMPLETCPSERVER_H

#include <QWidget>
#include <QTcpServer>

namespace Ui {
class SimpleTcpServer;
}

class SimpleTcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleTcpServer(QWidget *parent = nullptr);
    ~SimpleTcpServer();

private slots:
    void on_newConnection();
    void on_disconnected();
    void on_readyRead();
    void on_but_connect_clicked();

    void on_but_send_clicked();

private:
    Ui::SimpleTcpServer *ui;
    QTcpServer* m_server = nullptr;        // TCP服务端对象
    QTcpSocket* m_client = nullptr;        // 连接成功的客户端
};

#endif // SIMPLETCPSERVER_H
