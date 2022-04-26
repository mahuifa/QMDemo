#include "comchange.h"
#include <QApplication>
#include <QMutex>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug.h>
#ifdef Q_OS_WIN
#include <windows.h>
#include "dbt.h"
#pragma comment(lib, "user32.lib")
#endif

ComChange* ComChange::m_comChange = nullptr;
ComChange *ComChange::getInstance()
{
    if(m_comChange == nullptr)
    {
        static QMutex mutex;         //实例互斥锁。
        QMutexLocker locker(&mutex); //加互斥锁。
        if(m_comChange == nullptr)
        {
            m_comChange = new ComChange();
        }
    }
    return m_comChange;

}

/**
 * @brief   获取系统中所有可用的串口名
 * @return
 */
QStringList ComChange::getAvailablePort()
{
    QStringList strName;
    foreach(const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
    {
        QSerialPort port(info);
        if(port.open(QIODevice::ReadWrite))
        {
            strName << info.portName();
            port.close();
        }
    }
    return strName;
}


/**
 * @brief       设置窗口句柄用于过滤事件
 * @param hwnd
 */
void ComChange::setWid(quintptr wid)
{
    m_wid = wid;
}


ComChange::ComChange(QObject *parent) : QObject(parent)
{
    qApp->installNativeEventFilter(this);        // 安装事件过滤器

}

bool ComChange::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    Q_UNUSED(result)
    if(eventType == "windows_generic_MSG")           // 处理windows消息
    {
        MSG* msg = reinterpret_cast<MSG*>(message);

        if(msg->message == WM_DEVICECHANGE               // 通知应用程序设备或计算机的硬件配置发生更改。
          && msg->hwnd == (HWND)this->m_wid)                  // 过滤事件
        {
            PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
            switch (msg->wParam)
            {
            case DBT_DEVICEARRIVAL:             // 插入
            {
                if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT)           // 设备类型为串口
                {
                    PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                    QString strName = QString::fromWCharArray(lpdbv->dbcp_name);  //插入的串口名
                    emit comStatus(strName, true);
                }
                break;
            }
            case DBT_DEVICEREMOVECOMPLETE:      // 拔出
            {
                if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT)           // 设备类型为串口
                {
                    PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                    QString strName = QString::fromWCharArray(lpdbv->dbcp_name);  //拔出的串口名
                    emit comStatus(strName, false);
                }
                break;
            }
            default:
                break;
            }
        }
    }
#endif

    return false;
}
