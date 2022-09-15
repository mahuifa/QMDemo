/******************************************************************************
 * @文件名     readthread.h
 * @功能       读取视频图像数据线程，在线程中解码视频
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/09/15
 * @备注
 *****************************************************************************/
#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>

class ReadThread : public QThread
{
    Q_OBJECT
public:
    explicit ReadThread(QObject *parent = nullptr);

signals:

public slots:
};

#endif // READTHREAD_H
