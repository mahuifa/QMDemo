#include "widget.h"
#include "ui_widget.h"
#include <QtConcurrent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("QtConcurrent::map使用示例--V%1").arg(APP_VERSION));

    connect(&m_futureWatcher, &QFutureWatcher<void>::started, [](){qDebug() << "开始监测";});
    connect(&m_futureWatcher, &QFutureWatcher<void>::finished, [&]()
                              {
                                  qDebug() << "执行完成";
                                  m_tests.clear();         // 清空数组
                              });
    connect(&m_futureWatcher, &QFutureWatcher<void>::progressRangeChanged, [](int minimum, int maximum)          // 进度范围改变时触发信号
                              {
                                  qDebug() << QString("进度范围[%1,%2]").arg(minimum).arg(maximum);
                              });
    connect(&m_futureWatcher, &QFutureWatcher<void>::progressValueChanged, ui->progressBar, &QProgressBar::setValue);   // 进度改变时触发信号（会限制进度信号发射速率，可能无法接收到所有进度）
    connect(&m_futureWatcher, &QFutureWatcher<void>::progressRangeChanged, ui->progressBar, &QProgressBar::setRange);

}

Widget::~Widget()
{
    delete ui;
}


void test1(int value)
{
    qDebug() << "执行数字：" << value << " thread" << QThread::currentThreadId();
    QThread::sleep(1);
}
static QVector<int> datas{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 100, 200};

/**
 * @brief 多线程环境下高效地处理大量数据
 */
void Widget::on_pushButton_clicked()
{
    QFuture<void> f1 = QtConcurrent::map(datas, test1);
    m_futureWatcher.setFuture(f1);
}



void test2(TestClass* test)
{
    test->run();       // 执行成员函数
    delete test;       // 释放成员对象
}

/**
 * @brief 多线程环境下批量执行成员函数
 */
void Widget::on_pushButton_2_clicked()
{
    for(int i = 0; i< 10; i++)
    {
        m_tests.append(new TestClass());
    }

    QFuture<void> f1 = QtConcurrent::map(m_tests, test2);
    m_futureWatcher.setFuture(f1);
}
