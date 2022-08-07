/******************************************************************************
 * @文件名       serialportstatus.h
 * @功能      通过继承于QAbstractNativeEventFilter来监测串口热插拔由于windows消息会发给每一个窗口，
 *           所以如果打开多个窗或者使用到如QComboBox之类的控件，当串口插入或者拔出时会触发多次事件，
 *           在这里我使用的解决方法时传入一个窗口句柄，通过窗口句柄过滤事件，但不知道有没有其它更好的办法。
 *
 * @开发者     （作者）
 * @时间       2021/11/15
 * @备注
 *****************************************************************************/
#ifndef SERIALPORTSTATUS_H
#define SERIALPORTSTATUS_H

#include <QAbstractNativeEventFilter>
#include <QObject>

class SerialPortStatus : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit SerialPortStatus(QObject *parent = nullptr);

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

signals:

};

#endif // SERIALPORTSTATUS_H
