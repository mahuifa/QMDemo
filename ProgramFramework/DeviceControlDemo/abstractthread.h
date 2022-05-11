/******************************************************************************
 * @文件名     abstractthread.h
 * @功能       处理数据的线程接口类
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/05/10
 * @备注
 *****************************************************************************/
#ifndef ABSTRACTTHREAD_H
#define ABSTRACTTHREAD_H

#include <QObject>

class QThread;
class AbstractThread : public QObject
{
    Q_OBJECT
public:
    explicit AbstractThread(QObject *parent = nullptr);
    virtual ~AbstractThread();

protected:
    virtual void handleData(QByteArray arr) = 0;      // 处理传入的数据，通过子类重写用于处理不同的数据

signals:
    /**
     * @brief      通过信号将数据传入线程，否则线程无效（由于是跨线程，所以自带信号队列）
     * @param arr
     */
    void inputData(QByteArray arr);
    /**
     * @brief      将在线程中处理完成的数据通过信号传出
     * @param arr
     */
    void outputData(QByteArray arr);

protected:
    QThread* m_thread = nullptr;
};

#endif // ABSTRACTTHREAD_H
