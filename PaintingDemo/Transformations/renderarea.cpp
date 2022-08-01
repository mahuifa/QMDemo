#include "renderarea.h"
#include <QDebug>
#include <qpainter.h>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(200, 200);

    // 设置背景填充色
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);

    QFont newFont = this->font();
    newFont.setPixelSize(20);    // 设置字体大小
    this->setFont(newFont);      // 设置当前类使用的字体

    QFontMetrics fontMetrics(newFont);  // QFontMetrics 类提供字体度量信息。
    m_fontRectX = fontMetrics.boundingRect("X");    // 返回由 text 指定的字符串中字符的边界矩形
    m_fontRectY = fontMetrics.boundingRect("Y");
}

/**
 * @brief       设置用于绘制的图案路径
 * @param shape
 */
void RenderArea::setShape(const QPainterPath &shape)
{
    this->m_shape = shape;
    this->update();
}

/**
 * @brief            设置绘制图案的操作
 * @param operations
 */
void RenderArea::setOperations(const QList<Operation> &operations)
{
    this->m_operations = operations;
    this->update();
}

/**
 * @brief        重绘事件函数
 * @param event
 */
void RenderArea::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);        // 创建一个画家对象
    painter.setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿

    painter.translate(60, 60);         // 坐标原点偏移到(60, 60)位置
    drawOutline(painter);              // 画一个虚线矩形框

    painter.save();                    // 保存当前绘制状态，save()后所有QPainter设置在restore（）后将失效(比如画笔设置)
    transformPainter(painter);         // 设置偏移、缩放、旋转
    drawCoordinates(painter);          // 绘制坐标系
    painter.fillPath(m_shape, Qt::blue);
    painter.restore();
}

/**
 * @brief           画一个虚线矩形框
 * @param painter
 */
void RenderArea::drawOutline(QPainter &painter)
{
    painter.save();                   // 将QPainter状态保存，后面的设置不会产生影响

    painter.setPen(Qt::darkBlue);     // 设置画笔颜色
    painter.setPen(Qt::DashLine);     // 画笔样式为虚线
    painter.drawRect(0, 0, 100,100);  // 画虚线矩形

    painter.restore();
}

/**
 * @brief          在这个函数中设置画家对象的坐标原点偏移、旋转、缩放
 * @param painter
 */
void RenderArea::transformPainter(QPainter &painter)
{
    for(auto operation : m_operations)
    {
        switch (operation)
        {
        case Translate:
            painter.translate(50, 50);    // 坐标原点偏移 50 像素
            break;
        case Scale:
            painter.scale(0.75, 0.75);    // 缩小到原来的75%大小
            break;
        case Rotate:
            painter.rotate(60);           // 顺时针旋转60度
            break;
        default:break;
        }
    }
}

/**
 * @brief          绘制当前画图使用的坐标系
 * @param painter
 */
void RenderArea::drawCoordinates(QPainter &painter)
{
    painter.save();

    painter.setPen(Qt::red);
    painter.drawLine(0, 0, 50, 0);        // 绘制X轴直线
    painter.drawLine(45, -3, 50, 0);      // 画X轴箭头
    painter.drawLine(45, 3, 50, 0);
    painter.drawText(QRect(50, 0, m_fontRectX.width(), -m_fontRectX.height()), Qt::AlignCenter, "X");

    painter.drawLine(0, 0, 0, 50);        // 绘制Y轴
    painter.drawLine(3, 45, 0, 50);       // 画Y轴箭头
    painter.drawLine(-3, 45, 0, 50);
    painter.drawText(QRect(0, 50, -m_fontRectX.width(), m_fontRectX.height()), Qt::AlignCenter, "Y");


    painter.restore();
}
