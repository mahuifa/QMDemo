/******************************************************************************
 * @文件名     windowrect.h
 * @功能       选中鼠标当前位置窗口的类
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/11/19
 * @备注
 *****************************************************************************/
#ifndef WINDOWRECT_H
#define WINDOWRECT_H

#include <QTimer>
#include <QWidget>

class WindowRect : public QWidget
{
    Q_OBJECT
public:
    explicit WindowRect(QWidget *parent = nullptr);
    ~WindowRect() override;

signals:
    void selectRect(QRect rect);

private slots:
    void on_mouseSignal(QEvent* event);
    void showEvent(QShowEvent *event)        override;
    void hideEvent(QHideEvent *event)        override;
    void mousePressEvent(QMouseEvent *event) override;

protected:
    void on_timeout();
private:
    QTimer m_timer;
};

/**
 *  全局鼠标事件单例信号类
 */
class MouseEvent : public QObject
{
    Q_OBJECT
public:
    static MouseEvent* getInstance()
    {
        static MouseEvent mouseEvent;
        return &mouseEvent;
    }

signals:
    void mouseSignal(QEvent* event);

private:
    MouseEvent(){}
};

#endif // WINDOWRECT_H
