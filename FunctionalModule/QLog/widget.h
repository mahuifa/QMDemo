#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
class Widget;
}

QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

private slots:

    void on_timer();

    void on_com_FileType_activated(int index);

    void on_but_thread_clicked();

private:
    bool m_quit = false;   // 退出线程
    Ui::Widget* ui;
    QTimer* m_timer;
};
#endif   // WIDGET_H
