#include <qtconcurrentmap.h>
#include <qthread.h>
#include <QCoreApplication>
#include <QDebug>

using namespace QtConcurrent;

/**
 * @brief          sum()函数每次返回结果，reduce函数将被调用一次，并且应该将中间值合并到结果变量中。（单线程执行）
 *                 QtConcurrent:：mappedReduced（）保证一次只有一个线程会调用reduce，因此不需要使用互斥锁来锁定结果变量。
 *                 QtConcurrent:：ReduceOptions枚举提供了一种控制减少顺序的方法。
 *                 如果使用QtConcurrent:：UnorderReduce（默认值），则顺序未定义，而QtConcCurrent:：OrderedReduce则确保按原始序列的顺序进行缩减。
 * @param arr      sum()函数的返回值
 * @param value    result()函数的返回值，将当前函数中的结果通过这个值返回出去
 */
void result1(quint64& arr, quint64 value)
{
    qDebug() << QString("-->同步计算:%1 + %2，线程：").arg(arr).arg(value).toStdString().data()
             << QThread::currentThreadId();
    arr += value;
    QThread::msleep(500);
}

void result2(QList<quint64>& arr, quint64 value)
{
    qDebug() << QString("-->同步计算，线程：").toStdString().data() << QThread::currentThreadId();
    arr.push_back(value + 10000);
    QThread::msleep(500);
}

/**
 * @brief      在线程中异步计算数据，并返回计算的结果（并行）
 * @param max
 * @return
 */
quint64 sum(quint64 max)
{
    qDebug() << QString("开始计算从0~%1的和，线程：").arg(max).toStdString().data() << QThread::currentThreadId();
    quint64 sum = 0;
    for (quint64 i = 0; i < max; i++)
    {
        sum += i;
    }

    qDebug() << QString("完成计算从0~%1的和为：%2，线程：").arg(max).arg(sum).toStdString().data()
             << QThread::currentThreadId();
    return sum;
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    QList<quint64> arr = {10, 15, 20, 25};
#if 1
    quint64 collage = mappedReduced(arr, sum, result1);
    qDebug() << "计算完成" << collage;
#else
    QList<quint64> collage = mappedReduced(arr, sum, result2);   // 返回值类型和result()函数第一个参数相同
    qDebug() << "计算完成";
    for (auto v : collage)
    {
        qDebug() << v;
    }
#endif

    return a.exec();
}
