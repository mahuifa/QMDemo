/******************************************************************************
 * @文件名     handletcpthread.h
 * @功能      解析tcp数据的线程
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/05/10
 * @备注
 *****************************************************************************/
#ifndef HANDLETCPTHREAD_H
#define HANDLETCPTHREAD_H

#include "abstractthread.h"

class handleTcpThread : public AbstractThread
{
    Q_OBJECT
public:
    explicit handleTcpThread(QObject *parent = nullptr);
    ~handleTcpThread();


protected:
    void handleData(QByteArray arr) override;      // 处理传入的数据，通过子类重写用于处理不同的数据

signals:

};

#endif // HANDLETCPTHREAD_H
