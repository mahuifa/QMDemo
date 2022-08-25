#include "renderarea.h"
#include <QDebug>
#include <qpainter.h>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    // 设置背景填充色
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);

    showData();
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
    showData();
    this->update();
}

void RenderArea::showData()
{
    qDebug() << "-----------------------打印信息-----------------------";
    qDebug() << "矩阵元素：" << QString("[%1, %2, %3][%4, %5, %6][%7, %8, %9]")
                .arg(m_transform.m11()).arg(m_transform.m12()).arg(m_transform.m13())    // 水平缩放、垂直剪切、水平投影
                .arg(m_transform.m21()).arg(m_transform.m22()).arg(m_transform.m23())    // 水平剪切、垂直缩放、垂直投影
                .arg(m_transform.m31()).arg(m_transform.m32()).arg(m_transform.m33());   // 水平平移、垂直平移、投影因子
    qDebug() << "转换类型：" << m_transform.type();
    qDebug() << "仿射变换：" << m_transform.isAffine();
    qDebug() << "单位矩阵（TxNone）：" << m_transform.isIdentity();
    qDebug() << "矩阵可逆：" << m_transform.isInvertible();
    qDebug() << "矩阵旋转：" << m_transform.isRotating();
    qDebug() << "矩阵缩放：" << m_transform.isScaling();
    qDebug() << "矩阵偏移：" << m_transform.isTranslating();

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
    QRect rect(-100, -100, 200, 200);
    painter.drawRect(rect);
    painter.setFont(QFont("黑体", 25, QFont::Bold));

    // 绘制文本
    painter.setPen(Qt::red);
    painter.drawText(rect.topLeft() + QPoint(20, 50), "文本");
}
