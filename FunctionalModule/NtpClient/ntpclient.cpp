#include "ntpclient.h"
#include <QDateTime>
#include <QUdpSocket>
#include <QDebug>
#include <QtEndian>
#include <QElapsedTimer>
#include <QMetaEnum>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif
#ifdef Q_OS_LINUX
#include <sys/time.h>
#endif

NtpClient::NtpClient(QObject *parent) : QObject(parent)
{
    m_socket = new QUdpSocket(this);
    connect(m_socket, &QUdpSocket::connected, this, &NtpClient::on_connected);
    connect(m_socket, &QUdpSocket::readyRead, this, &NtpClient::on_readData);
}

/**
 * @brief        连接Ntp服务器，端口号默认123
 * @param url    Ntp服务器IP地址或网址
 */
void NtpClient::connectServer(QString url)
{
    close();
    m_socket->connectToHost(url, 123);
}

void NtpClient::close()
{
    m_socket->abort();
}

void NtpClient::on_connected()
{
    qDebug() << "连接成功！";
    QMetaEnum m = QMetaEnum::fromType<QAbstractSocket::SocketState>();      // 获取QUdpSocket连接状态字符串
    emit updateData(QString("连接成功：%1  %2").arg(m_socket->peerName()).arg(m.key(m_socket->state())));
}

void NtpClient::getTime()
{
    sendData();
}

QByteArray toNtpPacket() {
    QByteArray result(40, 0);

    quint8 li = 0;                   // LI闰秒标识器，占用2个bit，0 即可；
    quint8 vn = 3;                   // VN 版本号，占用3个bits，表示NTP的版本号，现在为3；
    quint8 mode = 3;                 // Mode 模式，占用3个bits，表示模式。 3 表示 client， 2 表示 server
    quint8 stratum = 0;              // 系统时钟的层数，取值范围为1～16，它定义了时钟的准确度。层数为1的时钟准确度最高，准确度从1到16依次递减，层数为16的时钟处于未同步状态，不能作为参考时钟。
    quint8 poll = 4;                 // 轮询时间，即两个连续NTP报文之间的时间间隔(4-14)
    qint8 precision = -6;            // 系统时钟的精度,精确到秒的平方级（-6 到 -20）

    result[0] = char((li << 6) | (vn <<3) | (mode));
    result[1] = char(stratum & 0xff);

    result[2] = char(poll & 0xff);
    result[3] = char(precision & 0xff);

    qint64 currentLocalTimestamp = QDateTime::currentMSecsSinceEpoch();
    result.append((const char *)&currentLocalTimestamp, sizeof(qint64));

    return result;
}

/**
 * @brief 发送NTP请求帧
 */
void NtpClient::sendData()
{
    QByteArray arr = toNtpPacket();
    qint64 len = m_socket->write(arr);
    if(len != arr.count())
    {
        qWarning() << "发送NTP请求帧失败：" << arr.toHex(' ');
    }
}


/**
 * @brief     将QByteArray类型时间戳数据转换为整形并且进行大小端转换
 * @param bt
 * @return
 */
quint32 byteToUInt32(QByteArray bt) {
    if(bt.count() != 4) return 0;

    quint32 value;
    memcpy(&value, bt.data(), 4);

    return qToBigEndian(value);       // 大端转小端
}

/**
 * @brief      将Ntp时间戳转换成QDateTime可用的时间戳
 * @param bt
 * @return
 */
qint64 byte64ToMillionSecond(QByteArray bt) {
    qint64 second = byteToUInt32(bt.left(4));
    qint64 millionSecond = byteToUInt32(bt.mid(4, 4));
    return (second * 1000L) + ((millionSecond * 1000L) >> 32);
}

/**
 * @brief 接收返回的NTP数据帧并解析
 */
void NtpClient::on_readData()
{
    QElapsedTimer timer;       // 统计数据解析消耗的时间
    timer.start();

    QByteArray buf = m_socket->readAll();
    qint64 currentLocalTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();        // 客户端接收到响应报文时的时间戳 T4
    if(buf.count() < 48)          // Ntp协议帧长度为48字节
    {
        return;
    }

    QDateTime epoch(QDate(1900, 1, 1), QTime(0, 0, 0));           // ntp时间计时从1900年开始
    QDateTime unixStart(QDate(1970, 1, 1), QTime(0, 0, 0));       // UNIX操作系统考虑到计算机产生的年代和应用的时限综合取了1970年1月1日作为UNIX TIME的纪元时间(开始时间)
    qint64 unixDiff = epoch.msecsTo(unixStart);

    // 解析ntp协议中的时间
    qint64 referenceTimestamp = byte64ToMillionSecond(buf.mid(16, 8)) - unixDiff;           // 参考时间戳
    qint64 originTimestamp;                                                                 // 原始时间戳    T1
    memcpy(&originTimestamp, buf.mid(24, 8), 8);
    qint64 receiveTimestamp = byte64ToMillionSecond(buf.mid(32, 8)) - unixDiff;             // 接收时间戳   T2
    qint64 translateTimestamp = byte64ToMillionSecond(buf.mid(40, 8)) - unixDiff;           // 传送时间戳   T3

    QDateTime dateTime;

#if 0
    qDebug() << "-----------NTP协议中包含的所有时间-----------";
    dateTime.setMSecsSinceEpoch(referenceTimestamp);
    qDebug() << "参考时间戳：  " << dateTime.toString("yyyy-MM-dd HH:mm:ss zzz");
    dateTime.setMSecsSinceEpoch(originTimestamp);
    qDebug() << "原始时间戳T1：" << dateTime.toString("yyyy-MM-dd HH:mm:ss zzz");
    dateTime.setMSecsSinceEpoch(receiveTimestamp);
    qDebug() << "接收时间戳T2：" << dateTime.toString("yyyy-MM-dd HH:mm:ss zzz");
    dateTime.setMSecsSinceEpoch(translateTimestamp);
    qDebug() << "传送时间戳T3：" << dateTime.toString("yyyy-MM-dd HH:mm:ss zzz");
    dateTime.setMSecsSinceEpoch(currentLocalTimestamp);
    qDebug() << "本地时间戳T4：" << dateTime.toString("yyyy-MM-dd HH:mm:ss zzz");
    qDebug() << "------------------------------------------";
#endif

    QString strTime;
#if 1         // 计算方式1：时间差offset=（（T2-T1）+（T3-T4））/2      实际时间=程序处理时间（timer.elapsed()） + 接收数据时间T4 + 客户端与服务端的时间差（offset）
    qint64 currentLocalTimestamp1 = timer.elapsed() + currentLocalTimestamp + qint64((receiveTimestamp - originTimestamp + translateTimestamp - currentLocalTimestamp) / 2);

    dateTime.setMSecsSinceEpoch(currentLocalTimestamp1);
    strTime = dateTime.toString("yyyy-MM-dd HH:mm:ss zzz");
    emit updateData(strTime);
#else        // 计算方式2：往返时延Delay=（T4-T1）-（T3-T2）            实际时间=程序处理时间（timer.elapsed()） + 服务器数据发出时间（T3）+ 通信时延（Delay）
    qint64 currentLocalTimestamp2 = timer.elapsed() + translateTimestamp + (((currentLocalTimestamp - originTimestamp) - (translateTimestamp - receiveTimestamp)) / 2);
    dateTime.setMSecsSinceEpoch(currentLocalTimestamp2);
    strTime = dateTime.toString("yyyy-MM-dd HH:mm:ss zzz");
#endif
    qDebug() << strTime;
    setDateTime(dateTime);
}


/**
 * @brief           设置系统时间（注意：这个功能需要使用管理员权限或者超级用户权限）
 * @param dateTime
 */
void NtpClient::setDateTime(QDateTime& dateTime)
{
    QDate date = dateTime.date();
    QTime time = dateTime.time();
#ifdef Q_OS_WIN

    SYSTEMTIME system_time = {0};
    memset(&system_time, 0, sizeof(SYSTEMTIME));
    system_time.wYear = date.year();
    system_time.wMonth = date.month();
    system_time.wDay = date.day();
    system_time.wHour = time.hour();
    system_time.wMinute = time.minute();
    system_time.wSecond = time.second();
    system_time.wMilliseconds = time.msec();
    if (SetLocalTime(&system_time))            // 仅限于管理员。
    {
        emit updateData("设置时间成功！");
    }
    else
    {
        emit updateData("设置时间失败！");
    }
#endif

#ifdef Q_OS_LINUX
    struct tm tptr;
    struct timeval tv;

    tptr.tm_year = date.year() - 1900;            // 这里必须-1900，否则设置不成功
    tptr.tm_mon = date.month() - 1;               // 月份取值范围为[0-11]
    tptr.tm_mday = date.day();
    tptr.tm_hour = time.hour();
    tptr.tm_min = time.minute();
    tptr.tm_sec = time.second();

    tv.tv_sec = mktime(&tptr);                    // 将tptr赋值给tv_sec
    tv.tv_usec = time.msec() * 1000;              // 设置微秒值

    if (0 == settimeofday(&tv, NULL))            // 仅限于超级用户, 使用sudo ./NtpClient
    {
        emit updateData("设置时间成功！");
    }
    else
    {
        emit updateData("设置时间失败！");
    }
#endif
}

