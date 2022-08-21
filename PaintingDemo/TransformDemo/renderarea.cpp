#include "renderarea.h"

#include <qpainter.h>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    // 设置背景填充色
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);

}

RenderArea::~RenderArea()
{

}

/**
 * @brief             设置变换矩阵
 * @param transform
 */
void RenderArea::setTransform(const QTransform &transform)
{
    m_transform = transform;
    this->update();
}


void RenderArea::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    // 设置画笔颜色和笔刷颜色
    painter.setPen(QPen(Qt::blue, 3));
    painter.setBrush(Qt::cyan);

    QLine line(200, 50, 300, 50);
    painter.drawLine(line);                     // 绘制原始线段
    painter.drawLine(m_transform.map(line));    // 绘制通过 QTransform映射的线段

    painter.setTransform(m_transform);          // 设置QTransform

    // 画一个矩形
    QRect rect(200, 100, 200, 200);
    painter.drawRect(rect);
    painter.setFont(QFont("黑体", 25, QFont::Bold));

    // 绘制文本
    painter.setPen(Qt::red);
    painter.drawText(rect.topLeft() + QPoint(20, 50), "文本");
}
