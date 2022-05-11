/******************************************************************************
 * @文件名     tcpsensor.h
 * @功能       使用Tcp通信的传感器类
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/05/10
 * @备注
 *****************************************************************************/
#ifndef TCPSENSOR_H
#define TCPSENSOR_H

#include "abstractsensor.h"

class QTcpSocket;

class TcpSensor : public AbstractSensor
{
    Q_OBJECT
public:
    explicit TcpSensor(QObject *parent = nullptr);
    ~TcpSensor();

    void setTarget(const QString &hostName, quint16 port);  // 设置Tcp连接所需的IP和端口

    void open() override;                           // 开始连接Tcp
    void close() override;                          // 关闭通信接口
    void write(const QByteArray& arr) override;     // 发送数据

protected slots:
    void on_connected();
    void on_disconnected();
    void on_readyRead();

signals:

private:
    QString m_ip    = "127.0.0.1";       // ip地址
    quint16 m_port  = 6666;              // 端口号
    QTcpSocket* m_tcpClient = nullptr;   // Tcp通信对象
};

#endif // TCPSENSOR_H
