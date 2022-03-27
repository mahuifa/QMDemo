#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "porttest.h"
#include "mousekeytest.h"

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
    void on_but_port_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    PortTest* m_port = nullptr;
    MouseKeyTest* m_mouseKey = nullptr;
};
#endif // WIDGET_H
