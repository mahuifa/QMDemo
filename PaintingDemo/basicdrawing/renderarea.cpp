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
    m_pixmap.load("://1.png");
    m_pen.setWidth(10);              // 画笔默认宽度为10，便于观察效果
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
    painter.drawText(QRect(10, -30, 100, 25), Qt::AlignLeft, QString("当前角度：%1").arg(m_rotate));
    // 画原点
    painter.drawPoint(0, 0);
    painter.drawEllipse(-5, -5, 10, 10);
    painter.drawLine(QPoint(0, 0), QPoint(100, 0));   // 画X轴
    painter.drawLine(QPoint(0, 0), QPoint(0, 100));   // 画Y轴
    QPainterPath path;
    path.moveTo(100, -3);
    path.lineTo(106, 0);
    path.lineTo(100, 3);
    painter.drawPath(path);  // 画X轴箭头
    painter.rotate(90);
    painter.drawPath(path);  // 画Y轴箭头

    painter.restore();  // 恢复画家状态
}

/**
 * @brief          画主要图案
 * @param painter
 */
void RenderArea::drawPattern(QPainter &painter)
{
    painter.save();

    QRect rect(10, 20, 280, 260);
    static const QPoint points[4] = {
        QPoint(100, 280),
        QPoint(120, 30),
        QPoint(280, 60),
        QPoint(290, 270)
    };
    int startAngle = 20 * 16;       // 开始角度 startAngle和arcLength必须以1/16度指定
    int arcLength = 120 * 16;       // 弧线角度
    // 创建一个绘制路径
    QPainterPath path;
    path.moveTo(20, 180);    // 移动开始点
    path.lineTo(20, 30);    // 添加一条(80到30d)竖线
    path.cubicTo(QPoint(180, 0), QPoint(80, 80), QPoint(180, 180)); // 添加一条曲线
    switch (m_shape)
    {
    case Line:      // 绘制线段
    {
        painter.drawLine(rect.bottomLeft(), rect.topRight());
        break;
    }
    case Points:    // 绘制4个点
    {
        painter.drawPoints(points, 4);
        break;
    }
    case Polyline:  // 画多线段(折线)
    {
        painter.drawPolyline(points, 4);
        break;
    }
    case Polygon:   // 画多边形
    {
        painter.drawPolygon(points, 4);
        break;
    }
    case Rect:      // 画矩形
    {
        painter.drawRect(rect);
        break;
    }
    case RoundedRect:   // 画圆角矩形
    {
        painter.drawRoundedRect(rect, 25, 25, Qt::AbsoluteSize);    // 使用绝对值（像素）
//        painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);    // 指定相对于边界矩形的大小，通常使用百分比测量值。
        break;
    }
    case Ellipse:   // 画椭圆
    {
        painter.drawEllipse(rect);
        break;
    }
    case Arc:      // 从20度开始，画一个120的弧线（角度的正值表示逆时针方向，负值表示顺时针方向。零度位于3点钟位置）
    {
        painter.drawArc(rect, startAngle, arcLength);
        break;
    }
    case Chord:    // 绘制由给定矩形、startAngle和arcLength定义的弦
    {
        painter.drawChord(rect, startAngle, arcLength);
        break;
    }
    case Pie:     // 绘制饼图（扇形）
    {
        painter.drawPie(rect, startAngle, arcLength);
        break;
    }
    case Path:   // 绘制由QPainterPath组成的图形
    {
        painter.drawPath(path);
        break;
    }
    case Text:  // 绘制文本
    {
        painter.drawText(rect, Qt::AlignCenter, "Qt Text \n 绘制文本");
        break;
    }
    case Pixmap: // 绘制图片
    {
        painter.drawPixmap(10, 10, m_pixmap);
        break;
    }
    }


    QGradient *gradient = const_cast<QGradient*>(m_brush.gradient());
    if(gradient)
    {
        QPen pen = m_pen;
        pen.setColor(QColor(255, 0, 0));
        painter.setPen(pen);
        switch (gradient->type())
        {
        case QGradient::LinearGradient:
        {
            QLinearGradient* linearGradient = static_cast<QLinearGradient*>(gradient);
            painter.drawPoint(linearGradient->start());               // 线性 渐变开始点
            painter.drawPoint(linearGradient->finalStop());           // 线性渐变结束点
            break;
        }
        case QGradient::RadialGradient:
        {
            QRadialGradient* radialGradient = static_cast<QRadialGradient*>(gradient);
            painter.drawPoint(radialGradient->center());             // 径向渐变中心点
            painter.drawPoint(radialGradient->focalPoint());         // 径向渐变焦点
            break;
        }
        case QGradient::ConicalGradient:
        {
            QConicalGradient* radialGradient = static_cast<QConicalGradient*>(gradient);
            painter.drawPoint(radialGradient->center());             // 扇形渐变中心点
            break;
        }
        case QGradient::NoGradient:break;
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
