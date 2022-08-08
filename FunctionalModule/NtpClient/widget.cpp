#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QThread>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(&m_ntpClient, &NtpClient::updateTime, ui->textEdit, &QTextEdit::append);
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
