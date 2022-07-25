#include "circlewidget.h"

#include <qdebug.h>
#include <qpainter.h>

CircleWidget::CircleWidget(QWidget *parent) : QWidget(parent)
{

}

/**
 * @brief       设置是否使用浮点型数据类型来保存绘制图案的数据
 * @param flag  true：使用 false：不使用
 */
void CircleWidget::setFloatBased(bool flag)
{
    this->floatBased = flag;
    this->update();
}

/**
 * @brief      是否使用抗锯齿渲染
 * @param flag true：使用  false：不使用
 */
void CircleWidget::setAntialiased(bool flag)
{
    this->antialiased = flag;
    this->update();
}

/**
 * @brief        设置绘制的参数
 * @param value
 */
void CircleWidget::setFrame(int value)
{
    this->frameNo = value;
    this->update();
}

void CircleWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, antialiased);           // 设置抗锯齿（开启后线条会更加平滑）
    painter.drawRect(QRect(0, 0, this->width() - 1, this->height() - 1)); // 画一个边框
    painter.translate(this->rect().center());                             // 将坐标轴原点移动到中心位置

    for(int diameter = 0; diameter < 256; diameter += 9)                  // 由于QColor透明度通道最大值为255，这里为了方便也设置最大255
    {
        int delta = (diameter / 2) - frameNo;
        int alpha = 255 - (delta * delta) / 4 - diameter;                 // 计算透明度

        if(alpha <= 0) continue;                                          // 如果颜色透明度不为正值则不进行绘制

        painter.setPen(QPen(QColor(0, 127, 127, alpha), 3));              // 设置画笔，颜色透明度渐变
        if(floatBased)
        {
            painter.drawEllipse(QRectF(-diameter / 2.0, -diameter / 2.0, diameter, diameter));   // 使用浮点数进行绘制，绘制的圆会更加均匀
        }
        else
        {
            painter.drawEllipse(QRect(-diameter / 2, -diameter / 2, diameter, diameter));        // 使用整形进行绘制，比浮点数计算快，适用于频繁、大量的重绘，但绘制效果较差
        }
    }
}
