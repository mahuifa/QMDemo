#include "globalkeyevent.h"
#if defined(Q_OS_LINUX)
#include <QtConcurrent>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/record.h>     //  如果找不到可以安装sudo apt-get install xorg-dev
#include <X11/Xlibint.h>
#if 0
//#include <xcb/xcb_keysyms.h>           // sudo apt install libxcb-keysyms1-dev
//#include <xcb/xinput.h>                // sudo apt install libxcb-xinput-dev

#include <xkbcommon/xkbcommon.h>         // sudo apt install libxkbcommon-dev
#include <xkbcommon/xkbcommon-keysyms.h>
#endif

static XRecordContext g_context = 0;
static Display* g_display = nullptr;

static bool init()
{
    g_display =XOpenDisplay(nullptr);           // 打开与控制显示器的X服务器的连接，详细说明看【https://tronche.com/gui/x/xlib/display/opening.html】
    if(!g_display)
    {
        qWarning() << "连接X服务失败！";
        return false;
    }
    XRecordClientSpec clients = XRecordAllClients;     // 初始化 XRecordCreateContext 所需的 XRecordClientSpec 参数，XRecordAllClients 的意思是 "记录所有 X Client" 的事件
    XRecordRange*range = XRecordAllocRange();          // 创建 XRecordRange 变量，用于控制记录事件的范围
    if (!range)
    {
        qDebug() << "无法分配XRecordRange";
        return false;
    }

    // 会监听到 first - last之间并包含first和last的所有类型的事件
    memset(range, 0, sizeof(XRecordRange));
    range->device_events.first = KeyPress;
    range->device_events.last  = KeyRelease;
    // 根据上面的记录客户端类型和记录事件范围来创建　“记录上下文”
    // 然后把 XRecordContext 传递给 XRecordEnableContext 函数来开启事件记录循环
    g_context = XRecordCreateContext(g_display, 0, &clients, 1,&range, 1);
    XFree(range);
    if(g_context == 0)
    {
        qWarning() << "创建事件记录上下文失败！";
        return false;
    }
    XSync(g_display, false);              // XSync 的作用就是把上面的X 代码立即发给 X Server，这样 X Server 接受到事件以后会立即发送给 XRecord 的 Client 连接  True

    return true;
}

/**
 * @brief      处理鼠标事件的回调函数，将X11鼠标事件转换为Qt鼠标事件，通过单例类MouseEvent发送出去
 * @param ptr
 * @param data
 */
static void callback(XPointer ptr, XRecordInterceptData* data)
{
    Q_UNUSED(ptr)

    if (data->category == XRecordFromServer)
    {
        xEvent * event  = reinterpret_cast<xEvent*>(data->data);
//      qDebug() << QString("鼠标坐标：[%1, %2]").arg(event->u.keyButtonPointer.rootX).arg(event->u.keyButtonPointer.rootY);   // 获取鼠标坐标
        switch (event->u.u.type)            // 动作类型
        {
        case KeyPress:                   // 键盘按下
        {
//            QPoint point = QCursor::pos();  // 获取鼠标当前位置
            qDebug() << event->u.u.detail;
            switch (event->u.u.detail)      // 按键类型
            {
            case Button1:     // 左键按下
            {
//                emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                break;
            }
            case Button2:     // 中键按下
            {
//                emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier));
                break;
            }
            case Button3:     // 右键按下
            {
//                emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
                break;
            }
            case Button4:     // 向前滚动
            {
//                emit GlobalMouseEvent::getInstance()->wheelEvent(new QWheelEvent(point, 120, Qt::MiddleButton, Qt::NoModifier));
                break;
            }
            case Button5:     // 向后滚动
            {
//                emit GlobalMouseEvent::getInstance()->wheelEvent(new QWheelEvent(point, -120, Qt::MiddleButton, Qt::NoModifier));
                break;
            }
            default:
            {
                qDebug() << QString("未定义的按键：%1").arg(event->u.u.detail);   // 比如很多鼠标边上会多几个键
                break;
            }
            }
            break;
        }
        case KeyRelease:                             // 键盘释放
        {
//            QPoint point = QCursor::pos();  // 获取鼠标当前位置
            switch (event->u.u.detail)      // 按键类型
            {
            case Button1:   // 左键释放
            {
//                emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
                break;
            }
            case Button2:   // 中键释放
            {
//                emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier));
                break;
            }
            case Button3:   // 右键释放
            {
//                emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
                break;
            }
            case Button4:   // 向前滚动
            {
                break;
            }
            case Button5:   // 向后滚动
            {
                break;
            }
            default:
            {
//                qDebug() << QString("未定义的按键：%1").arg(event->u.u.detail);   // 比如很多鼠标边上会多几个键
            }
            }
            break;
        }
        default:
            break;
        }
    }

    XRecordFreeData(data);
}

/**
 * 调用 XRecordEnableContext 函数建立 XRecord 上下文
 * X Server 事件一旦发生就传递给事件处理回调函数
 * XRecordEnableContext 函数一旦调用就开始进入堵塞时的事件循环，直到线程或所属进程结束
 */
static void enableContext()
{
    Status ret = XRecordEnableContext(g_display, g_context,  callback, nullptr);
    XCloseDisplay(g_display);    // 关闭连接
    g_display = nullptr;
    qDebug() << QString("退出键盘事件监听：%1").arg(ret);
}
/**
 * @brief  安装全局键盘事件监听器
 * @return
 */
bool GlobalKeyEvent::installKeyEvent()
{
    bool ret = init();
    if(!ret) return false;
    QtConcurrent::run(enableContext);   // 由于XRecordEnableContext会一直阻塞，所以需要在线程中调用
    return true;
}

/**
 * @brief   卸载全局键盘事件监听器
 * @return
 */
bool GlobalKeyEvent::removeKeyEvent()
{
    if(g_context == 0) return false;

    Display* display = XOpenDisplay(nullptr);         // 这里需要单独建立一个连接来关闭监听，否则XRecordEnableContext不会退出
    if(!display)
    {
        qWarning() << "连接X服务失败！";
        return false;
    }
    XRecordDisableContext(display, g_context);
    XFlush(display);
    XSync(display, false);

    XRecordFreeContext(display, g_context);           // 释放监听上下文，否则XRecordEnableContext不会退出
    g_context = 0;
    XCloseDisplay(display);
    return true;
}
#endif
