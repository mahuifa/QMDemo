#ifndef WORK_H
#define WORK_H

#include <QObject>

#include <QDebug>
#include <QThread>

class WorkThread: public QThread
{
    Q_OBJECT
public:

    explicit WorkThread(QObject *parent = nullptr)
    {
        qDebug() << "创建线程";
    }
    ~WorkThread()
    {
        qDebug() << "析构线程";
    }
};

class Work1 : public QObject
{
    Q_OBJECT
public:
    explicit Work1(QObject *parent = nullptr);
    ~Work1();

signals:
    void start();
    void updateData(int num);

public slots:
    void delayCount();

private:
    WorkThread* m_thread = nullptr;
};

#endif // WORK_H
