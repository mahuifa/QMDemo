#include "widget.h"
#include "ui_widget.h"
#include <QThread>
#include <QtConcurrent>

using namespace QtConcurrent;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("QtConcurrent::run使用示例--V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}

void test1()
{
    qDebug() << "进入测试1函数";
    QThread::sleep(3);
    qDebug() << "离开测试1函数：" << QThread::currentThreadId();
}

void test2(QString value1, int value2)
{
    qDebug() << "进入测试2函数：" << value1 <<" " << value2;
    QThread::sleep(3);
    qDebug() << "离开测试2函数：" << QThread::currentThreadId();
}

void Widget::test3()
{
    qDebug() << "进入测试3函数";
    QThread::sleep(3);
    qDebug() << "离开测试3函数：" << QThread::currentThreadId();
}

void Widget::test4()
{
    qDebug() << "进入测试4函数";
    QThread::sleep(3);
    qDebug() << "离开测试4函数：" << QThread::currentThreadId();
}

/**
 * @brief 线程取自全局QThreadPool
 */
void Widget::on_pushButton_clicked()
{
    QFuture<void> f1 = run(test1);                                 // 在新的线程中异步执行无参全局函数
    QFuture<void> f2 = run(test2, QString("传入线程的参数"), 123);   // 在新的线程中异步执行有参全局函数（注意：参数2不能直接传字符串常量，需要传QString）
    QFuture<void> f3 = run(&Widget::test3);                        // 在新的线程中异步执行静态成员函数
    QFuture<void> f4 = run(this, &Widget::test4);                  // 在新的线程中异步执行成员函数
    QFuture<void> f5 = run([]()                                    // 在新线程中执行Lambda表达式
    {
        qDebug() << "进入测试Lambda表达式";
        QThread::sleep(3);
        qDebug() << "离开测试Lambda表达式：" << QThread::currentThreadId();
    });

#if 1    // QFuture的用法

    if (f1.isCanceled())
    {
        qDebug() << "并发操作已经取消";
    }
    if (f1.isFinished())
    {
        qDebug() << "并发操作已经完成";
    }
    if (f1.isRunning())
    {
        qDebug() << "并发操作正在执行";
    }
    if (f1.isPaused())
    {
        qDebug() << "并发操作已暂停";
    }
    if (f1.isStarted())
    {
        qDebug() << "并发操作已经启动";
    }
#endif
}

int test5()
{
    qDebug() << "进入测试5函数";
    QThread::sleep(3);
    qDebug() << "离开测试5函数：" << QThread::currentThreadId();

    return 123;
}

/**
 * @brief 等待线程执行完成，获取返回值
 */
void Widget::on_pushButton_2_clicked()
{
    QFuture<int> f1 = run(test5);                // 在新的线程中异步执行无参全局函数
    int ret = f1.result();                       // 等待线程结束，获取返回值（这个函数会阻塞当前线程）

    qDebug() << "返回结果为：" << ret;
}

/**
 * @brief 等待线程执行完成
 */
void Widget::on_pushButton_3_clicked()
{
    QFuture<int> f1 = run(test5);                // 在新的线程中异步执行无参全局函数
    f1.waitForFinished();                        // 等待线程结束（这个函数会阻塞当前线程）

    qDebug() << "执行完成";
}

/**
 * @brief 在指定线程池中运行
 */
void Widget::on_pushButton_4_clicked()
{
    QFuture<void> f1 = run(&m_pool, test1);                                 // 在新的线程中异步执行无参全局函数
    QFuture<void> f2 = run(&m_pool, test2, QString("传入线程的参数"), 123);   // 在新的线程中异步执行有参全局函数（注意：参数2不能直接传字符串常量，需要传QString）
    QFuture<void> f3 = run(&m_pool, &Widget::test3);                        // 在新的线程中异步执行静态成员函数
    QFuture<void> f4 = run(&m_pool, this, &Widget::test4);                  // 在新的线程中异步执行成员函数
    QFuture<void> f5 = run(&m_pool, []()                                    // 在新线程中执行Lambda表达式
    {
        qDebug() << "进入测试Lambda表达式";
        QThread::sleep(3);
        qDebug() << "离开测试Lambda表达式：" << QThread::currentThreadId();
    });
}
