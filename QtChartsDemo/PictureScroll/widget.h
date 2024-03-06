#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
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
    void on_pushButton_clicked();
    void on_timeout();

private:
    Ui::Widget *ui;
    QTimer m_timer;
};
#endif // WIDGET_H
