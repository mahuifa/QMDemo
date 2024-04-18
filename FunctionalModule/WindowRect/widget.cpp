#include "widget.h"
#include <qgridlayout.h>
#include <QDebug>

#if defined(Q_OS_WIN)
#    include <Windows.h>
#    include <windef.h>
#    include <QPushButton>
#elif defined(Q_OS_LINUX)
#    include <X11/Xatom.h>
#    include <X11/Xlib.h>
#elif defined(Q_OS_MAC)
#endif

#if defined(Q_OS_WIN)
static HHOOK g_hook = nullptr;

/**
 * @brief           处理鼠标事件的回调函数
 * @param nCode
 * @param wParam
 * @param lParam
 * @return
 */
LRESULT CALLBACK CallBackProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case WM_LBUTTONDOWN:   // 鼠标左键按下
        {
            POINT pos;
            bool ret = GetCursorPos(&pos);
            if (ret)
            {
                qDebug() << pos.x << " " << pos.y;
            }
            qDebug() << "鼠标左键按下";
            break;
        }
    default:
        break;
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);   // 注意这一行一定不能少，否则会出大问题
}
#endif

Widget::Widget(QWidget* parent)
    : QWidget(parent)
{
    this->setWindowTitle(QString("Qt-框选鼠标当前位置窗口范围 - V%1").arg(APP_VERSION));
#if defined(Q_OS_WIN)
    // linux下鼠标穿透要放在后面两行代码的全前面，否则无效(但是鼠标穿透了会导致一些奇怪的问题，如窗口显示不全，所以这里不使用)
    // windows下如果不设置鼠标穿透则只能捕获到当前窗口
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
#endif
    this->setWindowFlags(Qt::FramelessWindowHint);                          // 去掉边框、标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);                       // 背景透明
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);   // 设置顶级窗口，防止遮挡

#if defined(Q_OS_WIN)
    // 由于windows不透明的窗体如果不设置设置鼠标穿透WindowFromPoint只能捕捉到当前窗体，而设置鼠标穿透后想要获取鼠标事件只能通过鼠标钩子
    g_hook = SetWindowsHookExW(WH_MOUSE_LL, CallBackProc, GetModuleHandleW(nullptr), 0);   // 挂载全局鼠标钩子
    if (g_hook)
    {
        qDebug() << "鼠标钩子挂接成功,线程ID:" << GetCurrentThreadId();
    }
    else
    {
        qDebug() << "鼠标钩子挂接失败:" << GetLastError();
    }
#endif

    // 在当前窗口上增加一层QWidget，否则不会显示边框
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(new QWidget(), 0, 0, 1, 1);

    this->setStyleSheet(" background-color: rgba(58, 196, 255, 40); border: 2px solid rgba(58, 196, 255, 200);");   // 设置窗口边框样式 dashed虚线，solid 实线

    // 使用定时器定时获取当前鼠标位置的窗口位置信息
    connect(&m_timer, &QTimer::timeout, this, &Widget::on_timeout);
    m_timer.start(200);
}

Widget::~Widget()
{
#if defined(Q_OS_WIN)
    if (g_hook)
    {
        bool ret = UnhookWindowsHookEx(g_hook);
        if (ret)
        {
            qDebug() << "卸载鼠标钩子。";
        }
    }
#endif
}

void Widget::on_timeout()
{
    QPoint point = QCursor::pos();   // 获取鼠标当前位置
#if defined(Q_OS_WIN)
    POINT pos;
    pos.x = point.x();
    pos.y = point.y();

    HWND hwnd = nullptr;
    hwnd = WindowFromPoint(pos);   // 通过鼠标位置获取窗口句柄
    if (!hwnd)
        return;

    RECT lrect;
    bool ret = GetWindowRect(hwnd, &lrect);   //获取窗口位置
    if (!ret)
        return;

    QRect rect;
    rect.setX(lrect.left);
    rect.setY(lrect.top);
    rect.setWidth(lrect.right - lrect.left);
    rect.setHeight(lrect.bottom - lrect.top);
    this->setGeometry(rect);   // 设置窗口边框

#elif defined(Q_OS_LINUX)   // linux下使用x11获取的窗口大小有可能不太准确，例如浏览器的大小会偏小
    // 获取根窗口
    Display* display = XOpenDisplay(nullptr);
    Window rootWindow = DefaultRootWindow(display);

    Window root_return, parent_return;
    Window* children = nullptr;
    unsigned int nchildren = 0;
    // 函数详细说明见xlib文档：https://tronche.com/gui/x/xlib/window-information/XQueryTree.html
    // 该函数会返回父窗口的子窗口列表children，因为这里用的是当前桌面的根窗口作为父窗口，所以会返回所有子窗口
    // 注意：窗口顺序(z-order)为自底向上
    XQueryTree(display, rootWindow, &root_return, &parent_return, &children, &nchildren);

    QRect recte;   // 保存鼠标当前所在窗口的范围
    for (unsigned int i = 0; i < nchildren; ++i)
    {
        if (children[i] == this->winId())
            continue;   // 由于当前窗口一直在最顶层，所以这里要过滤掉当前窗口，否则一直获取到的就是当前窗口大小
        XWindowAttributes attrs;
        XGetWindowAttributes(display, children[i], &attrs);   // 获取窗口参数
        if (attrs.map_state == IsViewable)                    // 只处理可见的窗口， 三个状态：IsUnmapped, IsUnviewable, IsViewable
        {
#    if 0
            QRect rect(attrs.x + 1, attrs.y, attrs.width, attrs.height);  // 这里x+1防止全屏显示，如果不+1，设置的大小等于屏幕大小是会自动切换成全屏显示状态，后面就无法缩小了
#    else
            QRect rect(attrs.x, attrs.y, attrs.width, attrs.height);
#    endif
            if (rect.contains(point))   // 判断鼠标坐标是否在窗口范围内
            {
                recte = rect;   // 记录最后一个窗口的范围
            }
        }
    }
#    if 0   // 在linux下使用setGeometry设置窗口会有一些问题
    this->showNormal();               // 第一次显示是如果是屏幕大小，则后面无法缩小，这里需要设置还原
    this->setGeometry(recte);         // 设置窗口边框
#    else   // 使用setFixedSize+move可以避免这些问题
    this->move(recte.x(), recte.y());
    this->setFixedSize(recte.width(), recte.height());
#    endif
    //    qDebug() << this->rect() <<recte<< this->windowState();

    // 注意释放资源
    XFree(children);
    XCloseDisplay(display);

#elif defined(Q_OS_MAC)
#endif
}
