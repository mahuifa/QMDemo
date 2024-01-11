#ifndef WIDGET_H
#define WIDGET_H

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
    void on_but_savePath_clicked();

    void on_but_thread_clicked(bool checked);

    void on_but_threads_clicked(bool checked);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
