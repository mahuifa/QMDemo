#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "openglwidget.h"
#include "openglwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void rotateWidget();
    void rotateWindow();

private slots:
    void on_rotate();
    void on_timer();
    void on_checkBox_window_stateChanged(int arg1);

    void on_checkBox_widget_stateChanged(int arg1);

    void on_but_thread_clicked();

    void on_but_timer_clicked();

private:
    Ui::Widget *ui;
    OpenGLWidget* m_openglWidget = nullptr;
    OpenGLWindow* m_openglWindow = nullptr;

    bool m_start = false;
    bool m_isWidget = false;
    bool m_isWindow = false;

    QTimer m_timer;
};
#endif // WIDGET_H
