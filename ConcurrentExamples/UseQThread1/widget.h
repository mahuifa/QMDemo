#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "work1.h"
#include "work2.h"

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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    Work1* m_work1 = nullptr;
    Work2* m_work2 = nullptr;
    QThread* m_thread = nullptr;
};
#endif // WIDGET_H
