#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class TCPServer;
}

class TCPServer : public QWidget
{
    Q_OBJECT

public:
    explicit TCPServer(QWidget *parent = nullptr);
    ~TCPServer();

private slots:
    void on_newConnection();
    void on_acceptError(QAbstractSocket::SocketError socketError);
    void on_but_connect_clicked();
    void on_disconnected();
    void on_readyRead();

    void on_but_clearRecv_clicked();

    void on_but_clearSend_clicked();

    void on_check_hexRecv_clicked(bool checked);

    void on_check_hexSend_clicked(bool checked);

    void on_but_send_clicked();

private:
    void init();
    void connectSlots();
    void addPeer(const QString& peer);
    qint64 sendData(const QByteArray& data);

private:
    Ui::TCPServer *ui;

    QTcpServer* m_tcpServer = nullptr;
    QList<QTcpSocket*> m_tcpClients;        // 保存所有连接的客户端
};

#endif // TCPSERVER_H
