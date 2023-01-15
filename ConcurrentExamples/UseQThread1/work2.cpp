#include "work2.h"

#include <qthread.h>
#include <QDebug>

Work2::Work2(QObject *parent) : QObject(parent)
{
    connect(this, &Work2::start, this, &Work2::delayCount);
}

Work2::~Work2()
{
    qDebug() << "析构Work2";
}

void Work2::delayCount()
{
    emit updateData(10);
    QThread::sleep(5);          // 延时5秒，模拟耗时操作
    emit updateData(100);
}
