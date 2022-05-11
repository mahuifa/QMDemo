#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "abstractsensor.h"

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
    void on_outputData(QByteArray arr);
    void on_openState(bool flag);
    void on_but_init_clicked();

    void on_but_open_clicked();

    void on_but_close_clicked();

    void on_but_write_clicked();

    void on_but_delete_clicked();

private:
    Ui::Widget *ui;
    AbstractSensor* m_sensor = nullptr;
};
#endif // WIDGET_H
