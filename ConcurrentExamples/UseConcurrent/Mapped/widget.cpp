#include "widget.h"
#include "ui_widget.h"

#include <qthread.h>
#include <qtconcurrentmap.h>
#include <qthread.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("QtConcurrent::mapped使用示例--V%1").arg(APP_VERSION));

    for(int i = 10; i < 20; i++)
    {
        m_maxs.append(i);
    }

    connect(&m_futureWatcher, &QFutureWatcher<quint64>::started, [](){qDebug() << "开始监测";});
    connect(&m_futureWatcher, &QFutureWatcher<quint64>::finished, [&]()
                              {
                                  qDebug() << "执行完成";
                              });
    connect(&m_futureWatcher, &QFutureWatcher<quint64>::progressRangeChanged, [](int minimum, int maximum)          // 进度范围改变时触发信号
                              {
                                  qDebug() << QString("进度范围[%1,%2]").arg(minimum).arg(maximum);
                              });
    connect(&m_futureWatcher, &QFutureWatcher<quint64>::progressValueChanged, ui->progressBar, &QProgressBar::setValue);   // 进度改变时触发信号（会限制进度信号发射速率，可能无法接收到所有进度）
    connect(&m_futureWatcher, &QFutureWatcher<quint64>::progressRangeChanged, ui->progressBar, &QProgressBar::setRange);
    connect(&m_futureWatcher, &QFutureWatcher<quint64>::resultReadyAt, this, &Widget::on_resultReadyAt);    // 当有返回值时触发这个信号
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief      在线程中异步计算数据，并返回计算的结果
 * @param max
 * @return
 */
quint64 sum(int max)
{
    qDebug() << QString("开始计算从0~%1的和，线程：").arg(max) << QThread::currentThreadId();
    quint64 sum = 0;
    for(int i = 0; i < max; i++)
    {
        sum += i;
        QThread::msleep(100);
    }

    return sum;
}

/**
 * @brief              获取异步计算的返回值
 * @param resultIndex
 */
void Widget::on_resultReadyAt(int resultIndex)
{
    QString str = QString("计算0~%1的和：%2").arg(m_maxs.at(resultIndex)).arg(m_futureWatcher.resultAt(resultIndex));
    ui->textEdit->append(str);
}

/**
 * @brief 开始在线程中异步计算数据
 */
void Widget::on_pushButton_clicked()
{
    if(m_futureWatcher.isRunning())          // 判断是否正在运行
    {
        m_futureWatcher.cancel();            // 取消还没开始的异步任务，已经开始的不会发送进度和结果就绪信号
        m_futureWatcher.waitForFinished();   // 等待已经开始的异步任务完成
        return;
    }
    m_futureWatcher.setFuture(QtConcurrent::mapped(m_maxs, sum));
}
