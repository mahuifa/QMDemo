#include "widget.h"
#include "ui_widget.h"

#include <QSharedPointer>
#include <qdebug.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Qt网络编程Demo");
}

Widget::~Widget()
{
    on_pushButton_clicked();
    delete ui;
}

/**
 * @brief 打开任意多个TCP客户端界面
 */
void Widget::on_but_TCPClient_clicked()
{
    m_tcpClients.append(new TCPClient);
    m_tcpClients.last()->show();
}

/**
 * @brief 关闭并释放所有界面
 */
void Widget::on_pushButton_clicked()
{
    while (m_tcpClients.count())
    {
        delete m_tcpClients.first();
        m_tcpClients.removeFirst();
    }
}

