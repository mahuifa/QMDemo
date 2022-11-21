#include "screenrect.h"

#include <qapplication.h>
#include <qpainter.h>
#include <QDesktopWidget>
#include <qscreen.h>
#include <QDebug>
#include <QMouseEvent>

ScreenRect::ScreenRect(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlag(Qt::FramelessWindowHint);           // 去除标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);       // 背景透明
    QScreen * screen = QGuiApplication::primaryScreen();    // 获取默认屏幕
    m_path.addRect(screen->geometry());
    this->setGeometry(screen->geometry());
}

/**
 * @brief        鼠标选择的启点
 * @param event
 */
void ScreenRect::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_rect.setTopLeft(event->pos());
    }
    QWidget::mousePressEvent(event);
}

/**
 * @brief       鼠标释放时如果选择了区域则将选择的矩形发送出去
 * @param event
 */
void ScreenRect::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() == Qt::LeftButton) && (m_rect.x() > 0 || m_rect.y() > 0))  // 这里使用x >0,y>0判断是否选择区域，而不是使用!m_rect.isEmpty()判断，避免反着选
    {
        this->hide();
        emit this->selectRect(m_rect.normalized());   // 使用normalized防止出现反着选，例如从下往上选，从右往左选
        m_rect = QRect();      // 置为空
    }

    QWidget::mouseReleaseEvent(event);
}

/**
 * @brief       鼠标选择的终点
 * @param event
 */
void ScreenRect::mouseMoveEvent(QMouseEvent *event)
{
    if(m_rect.x() > 0 || m_rect.y() > 0)
    {
        m_rect.setBottomRight(event->pos());
        this->update();
    }
    QWidget::mouseMoveEvent(event);
}

/**
 * @brief           绘制全屏遮罩和选择的矩形框
 * @param event
 */
void ScreenRect::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setPen(QPen(QColor(85, 170, 255, 200), 1));
    painter.setBrush(QColor(85, 170, 255, 50));
    QPainterPath path;
    path.addRect(m_rect);
    painter.drawPath(m_path - path);       // 绘制选择的矩形框
}
