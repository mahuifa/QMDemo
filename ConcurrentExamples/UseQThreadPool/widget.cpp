#include "widget.h"
#include "ui_widget.h"

#include <QRunnable>
#include <qthread.h>
#include <QDebug>
#include <QThreadPool>
#include <QMutex>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt使用线程池QThreadPool--V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}


static int g_deleteNum = 0;
static QMutex g_mutex;
// 用于在线程池中执行异步任务的类
class Task : public QRunnable
{
public:
    ~Task() override
    {
        QMutexLocker locker(&g_mutex);
        qDebug() << "销毁任务对象，释放个数：" << ++g_deleteNum;
    }
    void run() override
    {
        QThread::sleep(1);
        qDebug() << "执行任务：" << QThread::currentThreadId() ;
    }
};

/**
 * @brief 使用全局线程池
 */
void Widget::on_pushButton_clicked()
{
    g_deleteNum = 0;
    QThreadPool* pool = QThreadPool::globalInstance();  // 获取全局线程池
    int taskCount = ui->spinBox->value();
    int threadCount = ui->spinBox_2->value();
    if(threadCount > 0)   // 如果为0则使用默认值
    {
        pool->setMaxThreadCount(threadCount);  // 设置最大线程个数，但一般不需要设置
    }
    for(int i = 0; i < taskCount; i++)
    {
        pool->start(new Task());    // 向线程池中添加任务
    }

    qDebug() << "最大线程数：" << pool->maxThreadCount();
//    pool->clear();          // 清除所有当前排队但未开始运行的任务
//    pool->waitForDone();    // 等待线程中所有任务执行完成（会阻塞界面）
    qDebug() << "任务过期时间：" << pool->expiryTimeout();
    qDebug() << "线程池工作线程的堆栈大小：" << pool->stackSize();   // 默认为0
}

/**
 * @brief 使用局部线程池
 */
void Widget::on_pushButton_2_clicked()
{
    g_deleteNum = 0;
    QThreadPool pool;
    int taskCount = ui->spinBox->value();
    int threadCount = ui->spinBox_2->value();
    if(threadCount > 0)   // 如果为0则使用默认值
    {
        pool.setMaxThreadCount(threadCount);  // 设置最大线程个数，但一般不需要设置
    }
    for(int i = 0; i < taskCount; i++)
    {
        pool.start(new Task());    // 向线程池中添加任务
    }

    qDebug() << "最大线程数：" << pool.maxThreadCount();
//    pool.clear();          // 清除所有当前排队但未开始运行的任务
//    pool.waitForDone();    // 等待线程中所有任务执行完成（会阻塞界面）
    qDebug() << "任务过期时间：" << pool.expiryTimeout();
    qDebug() << "线程池工作线程的堆栈大小：" << pool.stackSize();
}

/**
 * @brief 可能存在内存泄漏
 * 以超过cpu的线程数执行任务，释放是对象个数小于创建个数
 */
void Widget::on_pushButton_3_clicked()
{
    g_deleteNum = 0;
    QThreadPool* pool = QThreadPool::globalInstance();  // 获取全局线程池
    ui->spinBox->setValue(100);
    int cout = pool->maxThreadCount();
    qDebug() << "最大线程数：" << cout;
    ui->spinBox_2->setValue(cout * 2);  // 将最大线程数设置比实际线程数大

    for(int i = 0; i < 100; i++)    // 执行100个任务
    {
        pool->start(new Task());    // 向线程池中添加任务
    }


    // 如果在已经添加任务后修改最大线程个数，需要先调用 QThreadPool::clear() 函数清空任务队列，
    // 否则可能会导致已经加入任务队列但尚未执行的任务被丢弃，从而引发内存泄漏。
//    pool->clear();
    pool->setMaxThreadCount(cout * 2);  // 设置最大线程个数，但一般不需要设置
}

/**
 * @brief 存在内存泄漏
 *        当设置的最大线程数大于默认线程个数时将会存在内存泄漏
 *        局部QThreadPool会默认调用waitForDone()，防止函数退出时释放局部对象
 */
void Widget::on_pushButton_4_clicked()
{
    g_deleteNum = 0;
    QThreadPool pool;

    int cout = pool.maxThreadCount();
    qDebug() << "最大线程数：" << cout;
    ui->spinBox_2->setValue(cout * 2);  // 将最大线程数设置比实际线程数大

    for(int i = 0; i < 100; i++)
    {
        pool.start(new Task());    // 向线程池中添加任务
    }

    // 如果在已经添加任务后修改最大线程个数，需要先调用 QThreadPool::clear() 函数清空任务队列，
    // 否则可能会导致已经加入任务队列但尚未执行的任务被丢弃，从而引发内存泄漏。
//    pool.clear();
    pool.setMaxThreadCount(cout * 2);  // 设置最大线程个数，但一般不需要设置
}
