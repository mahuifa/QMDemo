#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QThread>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("NTP时间同步客户端 - V%1").arg(APP_VERSION));
    qApp->setStyleSheet("*{font: 9pt '宋体';}");

    connect(&m_ntpClient, &NtpClient::updateData, ui->textEdit, &QTextEdit::append);


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_but_getTime_clicked()
{
    m_ntpClient.getTime();
//    QThread::msleep(100);
//    m_ntpClient.getTime();
}

void Widget::on_but_connect_clicked()
{
    m_ntpClient.connectServer(ui->com_address->currentText());
}
