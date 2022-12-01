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
    void on_mouseEvent(QMouseEvent* event);
    void on_wheelEvent(QWheelEvent* event);
    void on_keyEvent(QKeyEvent* event);
    void on_but_mouseI_clicked();

    void on_but_mouser_clicked();

    void on_but_keyI_clicked();

    void on_but_KeyR_clicked();

private:
    Ui::Widget *ui;
};



#endif // WIDGET_H
