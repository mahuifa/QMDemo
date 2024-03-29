#include "widget.h"
#include "ui_widget.h"
#include "tcpsensor.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt多通信接口适配器程序Demo - V%1").arg(APP_VERSION));
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 显示处理好的数据
 */
void Widget::on_outputData(QByteArray arr)
{
    ui->textEdit->append(arr);
}

/**
 * @brief        接口打开关闭状态
 * @param flag
 */
void Widget::on_openState(bool flag)
{
    if(flag)
    {
        qDebug() << "打开接口！";
    }
    else
    {
        qDebug() << "关闭接口！";
    }
}


void Widget::on_but_init_clicked()
{
    if(m_sensor) return;         // 如果不为空则返回

    m_sensor = new TcpSensor(this);
    connect(m_sensor, &AbstractSensor::outputData, this, &Widget::on_outputData);
    connect(m_sensor, &AbstractSensor::openState, this, &Widget::on_openState);
}

/**
 * @brief 打开连接
 */
void Widget::on_but_open_clicked()
{
    if(!m_sensor) return;       // 如果没有初始化则返回

    if(m_sensor->Type() == AbstractSensor::TcpType)       // 如果是Tcp类型
    {
        ((TcpSensor*)m_sensor)->setTarget("127.0.0.1", 8888);
    }

    m_sensor->open();           // 打开连接
}

/**
 * @brief 关闭连接
 */
void Widget::on_but_close_clicked()
{
    m_sensor->close();
}

/**
 * @brief 发送数据
 */
void Widget::on_but_write_clicked()
{
    if(!m_sensor) return;              // 如果没有初始化则返回
    if(!m_sensor->isOpen()) return;    // 如果没有打开则返回

    static int index = 0;
    QByteArray dataBuf;
    dataBuf.append(QString("发送数据：%1").arg(index++));
    m_sensor->write(dataBuf);
}

/**
 * @brief 释放通信对象
 */
void Widget::on_but_delete_clicked()
{
    if(!m_sensor) return;

    delete m_sensor;
    m_sensor = nullptr;
}

