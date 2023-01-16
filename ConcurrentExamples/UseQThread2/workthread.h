#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkThread(QObject *parent = nullptr);
    ~WorkThread() override;

signals:
    void updateData(int num);

public slots:
    void abort();

protected:
    void run() override;

private:
    bool m_abort = false;
};

#endif // WORKTHREAD_H
