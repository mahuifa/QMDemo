/******************************************************************************
 * @文件名     abstractsensor.h
 * @功能       传感器通信接口类
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/05/10
 * @备注
 *****************************************************************************/
#ifndef ABSTRACTSENSOR_H
#define ABSTRACTSENSOR_H

#include <QObject>
#include "abstractthread.h"


class AbstractSensor : public QObject
{
    Q_OBJECT
public:
    enum SensorType           // 传感器通信类的类型
    {
        uncertain,           // 不确定类型
        TcpType,             // Tcp通信的传感器通信类型
        SerialPortType       // 串口通信的传感器通信类型
    };

public:
    explicit AbstractSensor(QObject *parent = nullptr);
    virtual ~AbstractSensor();

    SensorType Type() {return m_type;}                 // 返回当前对象的类型
    bool isOpen(){return m_open;}                      // 返回打开关闭状态

    virtual void open() = 0;                           // 打开通信接口
    virtual void close() = 0;                          // 关闭通信接口
    virtual void write(const QByteArray& arr) = 0;     // 发送数据

signals:
    /**
     * @brief        接口打开状态
     * @param flag   true：打开成功 false：打开失败
     */
    void openState(bool flag);
    /**
     * @brief        返回接收到并处理完成的数据
     * @param arr
     */
    void outputData(QByteArray arr);

protected:
    AbstractThread* m_handleThread = nullptr;     // 数据解析线程
    SensorType m_type = uncertain;                // 当前对象的类型
    bool m_open = false;                          // 打开关闭状态
};

#endif // ABSTRACTSENSOR_H
