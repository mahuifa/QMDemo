#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
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
    void windowLayout();       // 界面布局

protected:
    void showFullScreen();                // 覆盖全屏显示功能
    void showNormal();                    // 覆盖全屏显示还原功能
    void showEvent(QShowEvent *event)     override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

    void videoWidgetEvent(QEvent* event);             // 视频显示窗口事件处理

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
