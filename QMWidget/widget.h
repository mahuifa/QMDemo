#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mwidgetbase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public MWidgetBase
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void init();               // 初始化
    void connectSlot();        // 绑定信号槽
    void loadStyle();          // 加载样式表
    void setWindowTitle(const QString& title);


private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
