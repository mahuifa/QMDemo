#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "comchange.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("串口、鼠标、键盘热插拔监测demo - V%1").arg(APP_VERSION));
    m_port = new PortTest();
    m_mouseKey = new MouseKeyTest();
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->resize(500, 300);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_but_port_clicked()
{
    m_port->show();
}

void Widget::on_pushButton_2_clicked()
{
    m_mouseKey->show();
}

