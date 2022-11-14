#ifndef WIDGET_H
#define WIDGET_H

#include <QMouseEvent>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_mouseSignal(QEvent* event);
    void on_but_mouseI_clicked();

    void on_but_mouser_clicked();

private:
    Ui::Widget *ui;
};

/**
 *  全局鼠标事件单例信号类
 */
class MouseEvent : public QObject
{
    Q_OBJECT
public:
    static MouseEvent* getInstance()
    {
        static MouseEvent mouseEvent;
        return &mouseEvent;
    }

signals:
    void mouseSignal(QEvent* event);

private:
    MouseEvent(){}
};

#endif // WIDGET_H
