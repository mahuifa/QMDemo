#include "renderarea.h"

#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QTextDocument>
#include <QWheelEvent>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    // 设置背景填充色
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);
}

/**
 * @brief     设置坐标原点偏移值
 * @param dx
 * @param dy
 */
void RenderArea::setTranslate(qreal dx, qreal dy)
{
    m_offset.setX(dx);
    m_offset.setY(dy);
    this->update();
}

/**
 * @brief               设置渲染是否抗锯齿
 * @param transformed
 */
void RenderArea::setAntialiased(bool transformed)
{
    m_antialiased = transformed;
    this->update();
}

/**
 * @brief     设置画笔
 * @param pen
 */
void RenderArea::setPen(const QPen &pen)
{
    m_pen = pen;
    this->update();
}

/**
 * @brief        设置笔刷
 * @param brush
 */
void RenderArea::setBrush(const QBrush &brush)
{
    m_brush = brush;
    this->update();
}

/**
 * @brief         设置旋转角度
 * @param rotate
 */
void RenderArea::setRotate(qreal rotate)
{
    m_rotate = rotate;
    this->update();
}

/**
 * @brief        绘制主要图案
 * @param shape
 */
void RenderArea::setShape(RenderArea::Shape shape)
{
    m_shape = shape;
    this->update();
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    if(m_antialiased) // 设置绘制渲染抗锯齿
    {
        painter.setRenderHint(QPainter::Antialiasing);
    }
    painter.translate(m_offset);   // 偏移坐标原点
    painter.rotate(m_rotate);      // 旋转
    drawOrigin(painter);           // 原点画笔恒定不变
    painter.scale(m_zoom, m_zoom);
    painter.setPen(m_pen);
    painter.setBrush(m_brush);
    drawPattern(painter);
}

/**
 * @brief         画原点
 * @param painter
 */
void RenderArea::drawOrigin(QPainter &painter)
{
    painter.save();      // 保存画家状态

    painter.setPen(QPen(QColor(0, 0, 0), 2));
    painter.drawPoint(0, 0);
    painter.drawEllipse(-5, -5, 10, 10);

    painter.restore();  // 恢复画家状态
}

/**
 * @brief          画主要图案
 * @param painter
 */
void RenderArea::drawPattern(QPainter &painter)
{
    painter.save();

    QRect rect(10, 20, 80, 60);
    switch (m_shape)
    {
    case Line:
    {
        painter.drawLine(rect.bottomLeft(), rect.topRight());
        break;
    }
    case Points:
    {
        break;
    }
    case Polyline:
    {
        break;
    }
    case Polygon:
    {
        break;
    }
    case Rect:
    {
        break;
    }
    case RoundedRect:
    {
        break;
    }
    case Ellipse:
    {
        break;
    }
    case Arc:
    {
        break;
    }
    case Chord:
    {
        break;
    }
    case Pie:
    {
        break;
    }
    case Path:
    {
        break;
    }
    case Text:
    {
        break;
    }
    case Pixmap:
    {
        break;
    }
    }
    painter.restore();
}


/**
 * @brief        鼠标滚轮缩放事件
 * @param event
 */
void RenderArea::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)    // 放大
    {
        m_zoom += 0.2;
    }
    else                      // 缩小
    {
        m_zoom -= 0.2;
    }

    this->update();
    QWidget::wheelEvent(event);
}
