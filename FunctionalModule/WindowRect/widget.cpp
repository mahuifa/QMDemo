#include "widget.h"
#include <QDebug>
#include <qgridlayout.h>

#if defined(Q_OS_WIN)
#include <Windows.h>
#include <windef.h>
#elif defined(Q_OS_LINUX)
#elif defined(Q_OS_MAC)
#endif

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle(QString("Qt-框选鼠标当前位置窗口范围 - V%1").arg(APP_VERSION));
    this->setWindowFlags(Qt::FramelessWindowHint);       // 去掉边框、标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);    // 背景透明
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);  // 设置顶级窗口，防止遮挡

    // 在当前窗口上增加一层QWidget，否则不会显示边框
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(new QWidget(), 0, 0, 1, 1);

    this->setStyleSheet("border: 3px dashed  rgba(58, 196, 255, 200);"); // 设置窗口边框样式 dashed虚线，solid 实线

    // 使用定时器定时获取当前鼠标位置的窗口位置信息
    connect(&m_timer, &QTimer::timeout, this, &Widget::on_timeout);
    m_timer.start(50);

}

Widget::~Widget()
{
}

#if defined(Q_OS_WIN)
void Widget::on_timeout()
{
    POINT point;
    bool ret = GetCursorPos(&point);    // 获取鼠标当前位置
    if(ret)
    {
        HWND hwnd = nullptr;
        hwnd = WindowFromPoint(point);   // 通过鼠标位置获取窗口句柄
        if(!hwnd) return;

        RECT lrect;
        GetWindowRect(hwnd, &lrect);     //获取窗口位置
        if(!ret) return;

        QRect rect;
        rect.setX(lrect.left);
        rect.setY(lrect.top);
        rect.setWidth(lrect.right - lrect.left);
        rect.setHeight(lrect.bottom - lrect.top);
        this->setGeometry(rect);         // 设置窗口边框

    }
}
#elif defined(Q_OS_LINUX)
void Widget::on_timeout()
{
}
#elif defined(Q_OS_MAC)
#endif
