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

signals:

private:
    QThread* m_thread = nullptr;
    QTcpSocket* m_client = nullptr;
};

#endif // IOTHREAD_H
