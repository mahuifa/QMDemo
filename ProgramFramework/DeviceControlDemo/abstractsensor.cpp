#include "abstractsensor.h"
#include <QDebug>

AbstractSensor::AbstractSensor(QObject *parent) : QObject(parent)
{
    qDebug() << "创建一个AbstractSensor";
}

AbstractSensor::~AbstractSensor()
{
    delete m_handleThread;
    m_handleThread = nullptr;

    qDebug() << "~AbstractSensor()";
}
