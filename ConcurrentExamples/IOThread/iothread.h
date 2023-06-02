#ifndef IOTHREAD_H
#define IOTHREAD_H

#include <QTcpSocket>
#include <QThread>

class IOThread : public QObject
{
    Q_OBJECT
public:
    explicit IOThread(QObject *parent = nullptr);
    ~IOThread();

private:
    void on_started();
    void on_finished();
    void on_readyRead();
    void on_connected();
    void on_disconnected();
    void on_connectTcp(QString ip, ushort port);
    void on_sendData(QByteArray data);           // 发送数据

signals:
    void connectTcp(QString ip, ushort port);    // 发起连接
    void close();                             // 关闭连接
    void readyRead(QByteArray data);          // 返回数据
    void sendData(QByteArray data);           // 发送数据

private:
    QThread* m_thread = nullptr;
    QTcpSocket* m_client = nullptr;
};

#endif // IOTHREAD_H
