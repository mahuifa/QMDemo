#include "porttest.h"
#include "ui_porttest.h"
#include <qdebug.h>
#include "comchange.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#include "Dbt.h"
#pragma comment(lib, "user32.lib")
#endif

PortTest::PortTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortTest)
{
    ui->setupUi(this);

    this->setWindowTitle("获取所有可用串口；串口热插拔监测。");

    // 第二种方法
    ComChange::getInstance()->setWid(this->winId());
    connect(ComChange::getInstance(), &ComChange::comStatus, this, &PortTest::on_comStatus);
    QStringList strName = ComChange::getAvailablePort();              // 获取所有可用串口
    ui->comboBox->addItems(strName);
    ui->comboBox_2->addItems(strName);
    ui->comboBox_3->addItems(strName);
    ui->comboBox_4->addItems(strName);
}

PortTest::~PortTest()
{
    delete ui;
}


void PortTest::on_comStatus(QString name, bool flag)
{
    if(flag)              // 串口插入时自动添加串口名
    {
        if(ui->comboBox->findText(name) < 0)       // 判断新增的串口名称在下拉框中是否存在，如果不存在则添加
        {
            ui->comboBox->addItem(name);
        }
        if(ui->comboBox_2->findText(name) < 0)
        {
            ui->comboBox_2->addItem(name);
        }
        if(ui->comboBox_3->findText(name) < 0)
        {
            ui->comboBox_3->addItem(name);
        }
        if(ui->comboBox_4->findText(name) < 0)
        {
            ui->comboBox_4->addItem(name);
        }
    }
    else                  // 串口拔出时自动移除串口名
    {
        ui->comboBox->removeItem(ui->comboBox->findText(name));
        ui->comboBox_2->removeItem(ui->comboBox_2->findText(name));
        ui->comboBox_3->removeItem(ui->comboBox_3->findText(name));
        ui->comboBox_4->removeItem(ui->comboBox_4->findText(name));
    }
}

/**
 * @brief            第一种方法
 * @param eventType
 * @param message
 * @param result
 * @return
 */
bool PortTest::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    if(eventType == "windows_generic_MSG")
    {
        MSG* msg = reinterpret_cast<MSG*>(message);
        if(msg->message == WM_DEVICECHANGE)                // 通知应用程序设备或计算机的硬件配置发生更改。
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
                    qDebug() << strName;
                }
                break;
            }
            case DBT_DEVICEREMOVECOMPLETE:      // 拔出
            {
                if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT)           // 设备类型为串口
                {
                    PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                    QString strName = QString::fromWCharArray(lpdbv->dbcp_name);  //拔出的串口名
                    qDebug() << strName;
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
