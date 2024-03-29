#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QWebSocket客户端简单示例 - V%1").arg(APP_VERSION));  // 设置窗口标题

    connect(&m_client, &QWebSocket::connected, this, &Widget::on_connected);
    connect(&m_client, &QWebSocket::textFrameReceived, this, &Widget::on_textMessageReceived);
    connect(&m_client, &QWebSocket::disconnected, this, &Widget::on_disconnected);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connected()
{
    ui->textEdit->append("连接成功！");
    ui->but_connect->setText("断开连接");
}

void Widget::on_disconnected()
{
    ui->textEdit->append("断开连接！");
    ui->but_connect->setText("连接");
}

/**
 * @brief          接收数据
 * @param message
 */
void Widget::on_textMessageReceived(const QString &message)
{
    ui->textEdit->append(message);
}

/**
 * @brief 连接/断开连接控制
 */
void Widget::on_but_connect_clicked()
{
    if(ui->but_connect->text() == "连接")
    {
        m_client.open(QUrl(ui->lineEdit_url->text().trimmed()));
    }
    else
    {
        m_client.close();
    }
}

/**
 * @brief 发送数据
 */
void Widget::on_but_send_clicked()
{
    if(m_client.state() == QAbstractSocket::ConnectedState)    // 判断是否连接
    {
        m_client.sendTextMessage(ui->lineEdit_send->text());
    }
}
