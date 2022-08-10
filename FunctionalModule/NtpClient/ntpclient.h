#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <QObject>
class QUdpSocket;

#if 0   // NTP协议帧（未使用）
typedef struct
{
    char  LI_VN_Mode;
    char  Stratum;
    char  Poll;
    char  Precision;
    int  RootDelay;
    int  RootDispersion;
    int  ReferenceIdentifier;
    quint64  ReferenceTimeStamp;    // 系统时钟最后一次被设定或更新的时间
    quint64   OriginateTimeStamp;    // NTP请求报文离开发送端时发送端的本地时间
    quint64   ReceiveTimeStamp;      // NTP请求报文到达Server端时接收端的本地时间。
    quint64   TransmitTimeStamp;     // 发送时间戳，客户端发送时填写，server接收到后会将TransmitTimeStamp值写入OriginateTimeStamp，然后NTP应答报文离开Server时在OriginateTimeStamp的本地时间。
}NtpPacket;
#endif

class NtpClient : public QObject
{
    Q_OBJECT
public:
    explicit NtpClient(QObject *parent = nullptr);

    void connectServer(QString url);        // 连接Ntp服务
    void close();
    void getTime();

signals:
    void updateData(const QString& time);          // 添加显示到界面上文本框中的信息

private slots:
    void on_connected();
    void on_readData();
    void sendData();
    void setDateTime(QDateTime& dateTime);

private:
    QUdpSocket* m_socket = nullptr;
};

#endif // NTPCLIENT_H
