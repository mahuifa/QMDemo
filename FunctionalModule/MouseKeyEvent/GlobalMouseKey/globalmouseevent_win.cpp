#include "globalmouseevent.h"
#if defined(Q_OS_WIN)
#include <QDebug>
#include <QCursor>
#include <QMouseEvent>

#include "Windows.h"


static HHOOK g_hook = nullptr;
/**
 * @brief           处理鼠标事件的回调函数，由于这不是一个成员函数，所以需要通过中间单例类mouseEvent将鼠标信号传递出来
 *                  具体内容看https://learn.microsoft.com/zh-cn/previous-versions/windows/desktop/legacy/ms644986(v=vs.85)
 * @param nCode     挂钩过程用于确定如何处理消息的代码。如果nCode小于零，则挂钩过程必须将消息传递给 CallNextHookEx 函数而不进行进一步处理，并且应返回CallNextHookEx返回的值
 * @param wParam    信号类型：WM_LBUTTONDOWN、WM_LBUTTONUP、WM_MOUSEMOVE、WM_MOUSEWHEEL、WM_MOUSEHWHEEL、WM_RBUTTONDOWN 或WM_RBUTTONUP。
 *                  其它扩展按键可以看https://learn.microsoft.com/en-us/windows/win32/inputdev/mouse-input
 * @param lParam    MSLLHOOKSTRUCT结构体指针
 * @return
 */
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    QPoint point = QCursor::pos();  // 获取鼠标当前位置
    switch (wParam)
    {
    case WM_LBUTTONDOWN:   // 鼠标左键按下
        emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
        break;
    case WM_MOUSEMOVE:     // 鼠标移动
        emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseMove, point, Qt::NoButton, Qt::NoButton, Qt::NoModifier));
        break;
    case WM_RBUTTONDOWN:   // 鼠标右键按下
        emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
        break;
    case WM_RBUTTONUP:     // 鼠标右键抬起
        emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
        break;
    case WM_LBUTTONUP:     // 鼠标左键抬起
        emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
        break;
    case WM_MOUSEWHEEL:    // 鼠标滚轮
    {
        MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
//        qDebug() << QString("坐标：(%1, %2)").arg(msll->pt.x).arg(msll->pt.y);     // 获取鼠标坐标
        int delta = GET_WHEEL_DELTA_WPARAM(msll->mouseData);                     // 获取滚轮状态，向前：120，向后-120
        emit GlobalMouseEvent::getInstance()->wheelEvent(new QWheelEvent(point, delta, Qt::MiddleButton, Qt::NoModifier));
        break;
    }
    case WM_MBUTTONUP:     // 鼠标中键释放
        emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier));
        break;
    case WM_MBUTTONDOWN:   // 鼠标中键按下
        emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier));
        break;
    case WM_XBUTTONDOWN:   // 鼠标功能键按下
    {
        MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        int Button = GET_XBUTTON_WPARAM (msll->mouseData);
        switch (Button)
        {
        case XBUTTON1:   // 后退
            emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::XButton1, Qt::XButton1, Qt::NoModifier));
            break;
        case XBUTTON2:   // 前进
            emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::XButton2, Qt::XButton2, Qt::NoModifier));
            break;
        }
        break;
    }
    case WM_XBUTTONUP:   // 鼠标功能键释放
    {
        MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        int Button = GET_XBUTTON_WPARAM (msll->mouseData);
        switch (Button)
        {
        case XBUTTON1:
            emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::XButton1, Qt::XButton1, Qt::NoModifier));
            break;
        case XBUTTON2:
            emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::XButton2, Qt::XButton2, Qt::NoModifier));
            break;
        }
        break;
    }
    default:
        break;
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);   // 注意这一行一定不能少，否则会出大问题
}


/**
 * @brief  安装全局鼠标事件监听器
 * @return
 */
bool GlobalMouseEvent::installMouseEvent()
{
    if(g_hook) return true;     // 避免重复安装
    /**
     * WH_KEYBOARD_LL 为全局键盘钩子, WH_MOUSE_LL 为全局鼠标钩子
     * 详细说明看官方文档：https://learn.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-setwindowshookexw
     */
    g_hook = SetWindowsHookExW(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandleW(nullptr), 0);
    return g_hook;
}

/**
 * @brief   卸载全局鼠标事件监听器
 * @return
 */
bool GlobalMouseEvent::removeMouseEvent()
{
    if(!g_hook) return true;   // 避免重复卸载
    bool ret = UnhookWindowsHookEx(g_hook);
    if(ret)
    {
        g_hook = nullptr;
        return true;
    }
    return false;
}

#endif
