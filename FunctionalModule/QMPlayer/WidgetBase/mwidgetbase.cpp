#include "mwidgetbase.h"

#ifdef Q_OS_WIN
#include <QEvent>
#include <qdebug.h>
#include <qgridlayout.h>
#include <qt_windows.h>
#include <windowsx.h>
#pragma comment(lib, "user32.lib")
#endif

MWidgetBase::MWidgetBase(QWidget *parent) : QWidget(parent)
{
    init();
}

MWidgetBase::~MWidgetBase()
{

}

void MWidgetBase::setTitleBar(QWidget *titleBar)
{
    m_titleBar = titleBar;
    m_titleBar->installEventFilter(this);         // 拦截标题栏事件
}

void MWidgetBase::init()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,3,0))                   // qt版本>=5.3可设置背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);         // 如果不在这设置透明，在widget中设置会使背景变黑
#endif
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);    // 设置窗口无边框

    this->installEventFilter(this);

#ifdef Q_OS_WIN
    HWND hwnd = (HWND)this->winId();
    // 设置窗口属性（winMouseEvent中实现标题栏功能需要）
    ::SetWindowLong(hwnd,                               // 当前窗口句柄
                    GWL_STYLE,                          // 设置窗口属性
                    ::GetWindowLong(hwnd, GWL_STYLE)    // 获取窗口属性
                    | WS_MAXIMIZEBOX                    // 创建带有最大化按钮的窗口
                    | WS_THICKFRAME                     // 窗口有一个大小调整边框
                    | WS_CAPTION);                      // 创建一个带标题栏的窗口(圆角效果)
#endif
}

bool MWidgetBase::eventFilter(QObject *watched, QEvent *event)
{
    thisEvent(watched, event);
    titleBarEvent(watched, event);
    return QWidget::eventFilter(watched, event);
}

/**
 * @brief            当前this对象事件处理
 * @param watched
 * @param event
 */
void MWidgetBase::thisEvent(QObject *watched, QEvent *event)
{
    if(watched != this)
    {
        return;
    }

    if (event->type() == QEvent::WindowStateChange)
    {
        emit windowStateChanged(this->windowState());    // 窗口状态变化
    }
}

/**
 * @brief             m_titleBar对象事件处理
 * @param watched
 * @param event
 */
void MWidgetBase::titleBarEvent(QObject *watched, QEvent *event)
{
    if(watched != m_titleBar)
    {
        return;
    }
}

bool MWidgetBase::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (eventType == "windows_generic_MSG")   // 处理windows事件
    {
        MSG* msg = static_cast<MSG*>(message);

        switch (msg->message)
        {
        case WM_NCCALCSIZE:       // 通过处理此消息，当窗口的大小或位置发生更改时，应用程序可以控制窗口的工作区的内容。(不能缺少)
            return true;
        case WM_NCHITTEST:
            return winMouseEvent(msg, result);
        case PBT_APMSUSPEND:         // 系统正在挂起操作
        case WM_POWERBROADCAST:      // 通知应用程序发生了电源管理事件
            this->showMinimized();   // 系统休眠的时候自动最小化可以规避程序可能出现的问题
            break;
        case PBT_APMRESUMEAUTOMATIC:    // 通知应用程序系统正在从睡眠或休眠状态中恢复
            this->showNormal();         //休眠唤醒后自动打开
            break;
        default:break;
        }
    }
    return false;
}

/**
 * @brief         处理鼠标事件，实现鼠标在窗口边缘缩放功能，标题栏拖动，双击全屏、还原功能
 * @param msg
 * @param result
 * @return
 */
bool MWidgetBase::winMouseEvent(MSG *msg, long *result)
{
    // 获取鼠标坐标
    int xPos = GET_X_LPARAM(msg->lParam);          // 低序位字指定光标的 x 坐标。 该坐标相对于屏幕的左上角。
    int yPos = GET_Y_LPARAM(msg->lParam);          // 高阶字指定光标的 y 坐标。 该坐标相对于屏幕的左上角。
    QPoint pos = mapFromGlobal(QPoint(xPos, yPos)); // 将全局坐标转换为窗口坐标

    // 实现标题栏功能
    if (m_titleBar
     && m_titleBar->rect().contains(pos)  // 如果鼠标在标题栏中移动则进行处理
     && !m_titleBar->childAt(pos))        // 如果当前鼠标位置没有子控件（如标题栏上的按键）则进行移动窗口处理
    {
        *result = HTCAPTION;              //(在标题栏中)实现windows中标题栏功能（如拖动，双击全屏、还原，拖动到边缘半屏全屏显示）
        return true;
    }

    //判断当前鼠标位置释放在窗口边缘
    bool left    = pos.x() < 6;
    bool right   = pos.x() > width() - 6;
    bool top     = pos.y() < 6;
    bool bottom  = pos.y() > height() - 6;

    //鼠标移动到四个角,这个消息是当鼠标移动或者有鼠标键按下时候发出的
    if (left && top)              // 在窗口边框的左上角。
    {
        *result = HTTOPLEFT;
    }
    else if (left && bottom)      // 在可调整大小的窗口的边框的左下角 (用户可以单击鼠标以沿对角) 调整窗口的大小
    {
        *result = HTBOTTOMLEFT;
    }
    else if (right && top)        // 在窗口边框的右上角。
    {
        *result = HTTOPRIGHT;
    }
    else if (right && bottom)     // 在可调整大小的窗口的边框右下角 (用户可以单击鼠标以沿对角) 调整窗口的大小。
    {
        *result = HTBOTTOMRIGHT;
    }
    else if (left)                // 在可调整大小的窗口的左边框中 (用户可以单击鼠标来水平调整窗口)
    {
        *result = HTLEFT;
    }
    else if (right)               // 在可调整大小的窗口的右边框 (用户可以单击鼠标来水平调整窗口) 。
    {
        *result = HTRIGHT;
    }
    else if (top)                 // 在窗口的上边框。
    {
        *result = HTTOP;
    }
    else if (bottom)              // 在可调整大小的窗口的下水平边框 (用户可以单击鼠标将窗口垂直调整) 。
    {
        *result = HTBOTTOM;
    }
    else
    {
        *result = 0;
    }

    if (*result)
    {
        return true;   // 将事件返回处理
    }

    return false;
}
