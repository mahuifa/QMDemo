#include "mousekeytest.h"
#include "ui_mousekeytest.h"

#include <qdebug.h>

#ifdef Q_OS_WIN
#include <windows.h>
#include "dbt.h"
#include "initguid.h"
#include "usbiodef.h"
#include "hidclass.h"
#include "ntddkbd.h"
#include "ntddmou.h"
#pragma comment(lib, "user32.lib")
#endif


MouseKeyTest::MouseKeyTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MouseKeyTest)
{
    ui->setupUi(this);

    this->setWindowTitle("鼠标、键盘热插拔监测。");
    registerGUID();
}

MouseKeyTest::~MouseKeyTest()
{
    delete ui;
}

/**
 * @brief  给设备注册通知
 */
void MouseKeyTest::registerGUID()
{
#ifdef Q_OS_WIN
    DEV_BROADCAST_DEVICEINTERFACE mouseInterface;
    ZeroMemory(&mouseInterface, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    mouseInterface.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    mouseInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    mouseInterface.dbcc_classguid = GUID_DEVINTERFACE_MOUSE ;
    if(!RegisterDeviceNotificationW((HANDLE)this->winId(), &mouseInterface, DEVICE_NOTIFY_WINDOW_HANDLE))
    {
        qDebug() << "鼠标GUID注册失败";
    }

    DEV_BROADCAST_DEVICEINTERFACE keyboardInterface;
    ZeroMemory(&keyboardInterface, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    keyboardInterface.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    keyboardInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    keyboardInterface.dbcc_classguid = GUID_DEVINTERFACE_KEYBOARD;
    if(!RegisterDeviceNotificationW((HANDLE)this->winId(), &keyboardInterface, DEVICE_NOTIFY_WINDOW_HANDLE))
    {
        qDebug() << "键盘GUID注册失败";
    }
#endif
}

/**
 * @brief
 * @param eventType
 * @param message
 * @param result
 * @return
 */
bool MouseKeyTest::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    if(eventType == "windows_generic_MSG")
    {
        MSG* msg = reinterpret_cast<MSG*>(message);
        if(msg->message == WM_DEVICECHANGE)                // 通知应用程序设备或计算机的硬件配置发生更改。
        {
            PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
            PDEV_BROADCAST_DEVICEINTERFACE lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
            switch (msg->wParam)
            {
            case DBT_DEVICEARRIVAL:             // 插入
            {
                if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                {
                    if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_KEYBOARD)
                    {
                        ui->textEdit->append("键盘插入：" + QString::fromWCharArray(lpdbv->dbcc_name));
                    }
                    else if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_MOUSE)
                    {
                        ui->textEdit->append("鼠标插入：" + QString::fromWCharArray(lpdbv->dbcc_name));
                    }
                }
                break;
            }
            case DBT_DEVICEREMOVECOMPLETE:      // 拔出
            {
                if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                {
                    if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_KEYBOARD)
                    {
                        ui->textEdit->append("键盘移除：" + QString::fromWCharArray(lpdbv->dbcc_name));
                    }
                    else if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_MOUSE)
                    {
                        ui->textEdit->append("鼠标移除：" + QString::fromWCharArray(lpdbv->dbcc_name));
                    }
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
