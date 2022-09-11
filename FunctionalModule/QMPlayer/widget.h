#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
#include <QWidget>
#include <qpropertyanimation.h>
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
    void dynamicShowNormal();
    void dynamicShow();
    void dynamicHide();

protected:
    void showEvent(QShowEvent *event)     override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

    void videoWidgetEvent(QEvent* event);             // 视频显示窗口事件处理

private:
    Ui::Widget *ui;

    QPropertyAnimation* m_paSlider = nullptr;              // 动画对象，负责打开关闭进度条窗口动画
    QPropertyAnimation* m_paControlBar = nullptr;          // 动画对象，负责打开关闭控制栏窗口动画
    QPropertyAnimation* m_paSidebar = nullptr;             // 动画对象，负责打开关闭侧边栏窗口动画
    bool m_visible = true;
};
#endif // WIDGET_H
