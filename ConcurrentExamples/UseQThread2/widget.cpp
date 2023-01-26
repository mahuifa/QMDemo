#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt通过子类化，继承QThread，重写run实现子线程--V%1").arg(APP_VERSION));

    m_workThread = new WorkThread(this);
    connect(m_workThread, &WorkThread::updateData, ui->lcdNumber, QOverload<int>::of(&QLCDNumber::display));
}

Widget::~Widget()
{
//    m_workThread->terminate();     // 直接终止线程，此功能很危险，不鼓励使用
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    m_workThread->start();
}

void Widget::on_pushButton_2_clicked()
{
    m_workThread->abort();
}
