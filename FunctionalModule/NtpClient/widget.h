#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ntpclient.h"

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
    void on_but_getTime_clicked();

    void on_but_connect_clicked();

private:
    Ui::Widget *ui;

    NtpClient m_ntpClient;
};
#endif // WIDGET_H
