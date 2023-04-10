#ifndef WIDGET_H
#define WIDGET_H

#include <QFutureWatcher>
#include <QWidget>
#include <QDebug>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class TestClass;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    QFutureWatcher<void> m_futureWatcher;   // 允许使用信号和插槽监控 QFuture
    QVector<TestClass*> m_tests;
};


class TestClass {
public:
    TestClass()
    {
        qDebug() << "创建TestClass";
    }
    ~TestClass()
    {
        qDebug() << "销毁TestClass";
    }

    void run()
    {
        qDebug() << "执行对象：" << this << " thread" << QThread::currentThreadId();
        QThread::sleep(1);
    }
};

#endif // WIDGET_H
