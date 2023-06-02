#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "iothread.h"

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
    void on_pushButton_clicked(bool checked);

    void on_pushButton_2_clicked();

private:
    void on_readyRead(QByteArray data);          // 返回数据

private:
    Ui::Widget *ui;
    IOThread* m_io = nullptr;
};
#endif // WIDGET_H
