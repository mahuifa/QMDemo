/******************************************************************************
 * @文件名     simpletcpclient.h
 * @功能       简易的TCP客户端功能Demo，只保留TCP连接/断开、数据发送、接收功能
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/17
 * @备注
 *****************************************************************************/
#ifndef SIMPLETCPCLIENT_H
#define SIMPLETCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class SimpleTcpClient;
}

class SimpleTcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleTcpClient(QWidget *parent = nullptr);
    ~SimpleTcpClient();

private slots:
    void on_connected();
    void on_disconnected();
    void on_readyRead();
    void on_but_connect_clicked();

    void on_but_send_clicked();

private:
    Ui::SimpleTcpClient *ui;

    QTcpSocket* m_tcpClient = nullptr;
};

#endif // SIMPLETCPCLIENT_H
