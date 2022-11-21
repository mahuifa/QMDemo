#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

#if defined(Q_OS_WIN)
#include "Windows.h"
#endif

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Qt-自定义全局鼠标监听Demo - V%1").arg(APP_VERSION));
    connect(MouseEvent::getInstance(), &MouseEvent::mouseSignal, this, &Widget::on_mouseSignal);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_mouseSignal(QEvent* event)
{
    QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);
    switch (event->type())
    {
    case QEvent::MouseButtonPress:   // 鼠标按下
    {
        QString but;
        switch (me->button())
        {
        case Qt::LeftButton:
        {
            but = "左键";
            break;
        }
        case Qt::RightButton:
        {
            but = "右键";
            break;
        }
        default:
        {
            but = "未知";
            break;
        }
        }
        QString str = QString("鼠标%1按下：(x:%2, y:%3)").arg(but).arg(me->x()).arg(me->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::MouseMove:     // 鼠标移动
    {
        QString str = QString("鼠标移动：(x:%1, y:%2)").arg(me->x()).arg(me->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::MouseButtonRelease:     // 鼠标右键抬起
    {
        QString but;
        switch (me->button())
        {
        case Qt::LeftButton:
        {
            but = "左键";
            break;
        }
        case Qt::RightButton:
        {
            but = "右键";
            break;
        }
        default:
        {
            but = "未知";
            break;
        }
        }
        QString str = QString("鼠标%1释放：(x:%2, y:%3)").arg(but).arg(me->x()).arg(me->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::Wheel:    // 鼠标滚轮
    {
        QWheelEvent* we = dynamic_cast<QWheelEvent*>(event);
        QString str = QString("鼠标滚轮：%1，(x:%2, y:%3)").arg(we->delta() > 0 ? "向前" : "向后").arg(we->x()).arg(we->y());
        ui->textEdit->append(str);
        break;
    }
    default:
        break;
    }

    delete event;
}

#if defined(Q_OS_WIN)
/**
 * @brief           处理鼠标事件的回调函数，由于这不是一个成员函数，所以需要通过中间单例类mouseEvent将鼠标信号传递出来
 *                  具体内容看https://learn.microsoft.com/zh-cn/previous-versions/windows/desktop/legacy/ms644986(v=vs.85)
 * @param nCode     挂钩过程用于确定如何处理消息的代码。如果nCode小于零，则挂钩过程必须将消息传递给 CallNextHookEx 函数而不进行进一步处理，并且应返回CallNextHookEx返回的值
 * @param wParam    信号类型：WM_LBUTTONDOWN、WM_LBUTTONUP、WM_MOUSEMOVE、WM_MOUSEWHEEL、WM_MOUSEHWHEEL、WM_RBUTTONDOWN 或WM_RBUTTONUP。
 * @param lParam    MSLLHOOKSTRUCT结构体指针
 * @return
 */
LRESULT CALLBACK MouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    QPoint point = QCursor::pos();  // 获取鼠标当前位置
    switch (wParam)
    {
    case WM_LBUTTONDOWN:   // 鼠标左键按下
        emit MouseEvent::getInstance()->mouseSignal(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
        break;
    case WM_MOUSEMOVE:     // 鼠标移动
        emit MouseEvent::getInstance()->mouseSignal(new QMouseEvent(QEvent::MouseMove, point, Qt::NoButton, Qt::NoButton, Qt::NoModifier));
        break;
    case WM_RBUTTONDOWN:   // 鼠标右键按下
        emit MouseEvent::getInstance()->mouseSignal(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
        break;
    case WM_RBUTTONUP:     // 鼠标右键抬起
        emit MouseEvent::getInstance()->mouseSignal(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
        break;
    case WM_LBUTTONUP:     // 鼠标左键抬起
        emit MouseEvent::getInstance()->mouseSignal(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
        break;
    case WM_MOUSEWHEEL:    // 鼠标滚轮
    {
        MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
//        qDebug() << QString("坐标：(%1, %2)").arg(msll->pt.x).arg(msll->pt.y);     // 获取鼠标坐标
        int delta = GET_WHEEL_DELTA_WPARAM(msll->mouseData);                     // 获取滚轮状态，向前：120，向后-120
        emit MouseEvent::getInstance()->mouseSignal(new QWheelEvent(point, delta, Qt::MiddleButton, Qt::NoModifier));
        break;
    }
    default:
        break;
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);   // 注意这一行一定不能少，否则会出大问题
}

static HHOOK g_hook = nullptr;
#endif
/**
 * @brief 安装全局鼠标钩子
 */
void Widget::on_but_mouseI_clicked()
{
#if defined(Q_OS_WIN)
    if(g_hook) return;     // 避免重复安装
    /**
     * WH_KEYBOARD_LL 为全局键盘钩子, WH_MOUSE_LL 为全局鼠标钩子
     * 详细说明看官方文档：https://learn.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-setwindowshookexw
     */
    g_hook = SetWindowsHookExW(WH_MOUSE_LL, MouseCallback, GetModuleHandleW(nullptr), 0);
    if (g_hook)
    {
        qDebug() << "鼠标钩子挂接成功,线程ID:" << GetCurrentThreadId();

    }
    else
    {
        qDebug() << "鼠标钩子挂接失败:" << GetLastError();
    }
#endif
}

/**
 * @brief 卸载鼠标钩子
 */
void Widget::on_but_mouser_clicked()
{
#if defined(Q_OS_WIN)
    if(!g_hook) return;   // 避免重复卸载
    bool ret = UnhookWindowsHookEx(g_hook);
    if(ret)
    {
        g_hook = nullptr;
        qDebug() << "卸载鼠标钩子。";
    }
#endif
}
