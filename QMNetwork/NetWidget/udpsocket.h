#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QWidget>
#include <QUdpSocket>

#define TEST1  0    // 使用第一种方式实现UDP通信

namespace Ui {
class UdpSocket;
}

class UdpSocket : public QWidget
{
    Q_OBJECT

public:
    explicit UdpSocket(QWidget *parent = nullptr);
    ~UdpSocket();

private:
    void init();
    void connectSlots();

private slots:
    void on_readyRead();
#if TEST1
    void on_connected();
#endif
    void on_but_connect_clicked();

    void on_com_type_activated(int index);

    void on_but_send_clicked();

    void on_but_clearRecv_clicked();

    void on_but_clearSend_clicked();

    void on_check_hexRecv_clicked(bool checked);

    void on_check_hexSend_clicked(bool checked);

private:
    Ui::UdpSocket *ui;

    QUdpSocket* m_udpSocket = nullptr;
};

#endif // UDPSOCKET_H
