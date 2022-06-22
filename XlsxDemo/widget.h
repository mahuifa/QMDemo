#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    enum DemoType {       // QXlsx演示Demo枚举
        Demo1,
        Demo2,
        Demo3
    };

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void createObject(DemoType type);

private slots:
    void on_but_demo1_clicked();

    void on_but_demo2_clicked();

    void on_but_demo3_clicked();

private:
    Ui::Widget *ui;

    QWidget* m_widget = nullptr;
};
#endif // WIDGET_H
