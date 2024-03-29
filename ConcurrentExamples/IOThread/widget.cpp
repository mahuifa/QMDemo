#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800, 600);
    this->setWindowTitle(QString("Qt在子线程中使用IO设备（TCP）--V%1").arg(APP_VERSION));
    m_io = new IOThread;    // 不能指定父对象
    connect(m_io, &IOThread::readyRead, this, &Widget::on_readyRead);
}

Widget::~Widget()
{
    delete m_io;
    delete ui;
}


void Widget::on_pushButton_clicked(bool checked)
{
    if(checked)
    {
        emit m_io->connectTcp(ui->line_ip->text(), ui->spin_port->value());
        ui->pushButton->setText("关闭连接");
    }
    else
    {
        emit m_io->close();
        ui->pushButton->setText("连接");
    }
}

/**
 * @brief 发送数据
 */
void Widget::on_pushButton_2_clicked()
{
    emit m_io->sendData(ui->textEdit_2->toPlainText().toUtf8());
}

/**
 * @brief       显示接收的数据
 * @param data
 */
void Widget::on_readyRead(QByteArray data)
{
    qDebug() << data;
    ui->textEdit->append(data);
}
