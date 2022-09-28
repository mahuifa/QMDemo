#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "openglwidget.h"

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
    void on_but_widget_clicked();

private:
    Ui::Widget *ui;
    OpenGLWidget* m_openglWidget = nullptr;
};
#endif // WIDGET_H
