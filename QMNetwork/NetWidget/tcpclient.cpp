#include "tcpclient.h"
#include "ui_tcpclient.h"

#include <QDebug>

TCPClient::TCPClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCPClient)
{
    ui->setupUi(this);
    this->setWindowTitle("TCP客户端Demo");
}

TCPClient::~TCPClient()
{
#ifdef QT_DEBUG
    qDebug() <<"~TCPClient()";
#endif
    delete ui;
}
