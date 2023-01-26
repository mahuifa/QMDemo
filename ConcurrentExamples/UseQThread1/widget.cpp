#include "widget.h"
#include "ui_widget.h"

#include <qthread.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt通过实例化QThread实现子线程--V%1").arg(APP_VERSION));

    // 将创建子线程步骤封装到Work1中
    m_work1 = new Work1();
    connect(m_work1, &Work1::updateData, ui->lcdNumber, QOverload<int>::of(&QLCDNumber::display));


    // 在外部创建子线程
    m_thread = new QThread();
    m_thread->start();
    m_work2 = new Work2();
    m_work2->moveToThread(m_thread);
    connect(m_thread, &QThread::finished, m_work2, &QObject::deleteLater);    // 在线程停止后释放Work2
    connect(m_work2, &Work2::updateData, ui->lcdNumber, QOverload<int>::of(&QLCDNumber::display));
}

Widget::~Widget()
{
    if(m_work1)
    {
        delete m_work1;
    }
    m_thread->quit();
    m_thread->wait();
    delete m_thread;          // 由于m_thread没有指定父对象，所以需要自己释放
    m_thread = nullptr;
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    m_work1->start();              // 使用信号调用则是异步（在子线程中）
//    m_work1->delayCount();         // 直接调用方法则是同步（在当前线程中）
}

void Widget::on_pushButton_2_clicked()
{
    m_work2->start();              // 使用信号调用则是异步（在子线程中）
//    m_work2->delayCount();         // 直接调用方法则是同步（在当前线程中）
}
