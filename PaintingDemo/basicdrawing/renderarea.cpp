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

void RenderArea::paintEvent(QPaintEvent *event)
{

}

/**
 * @brief        鼠标滚轮缩放事件
 * @param event
 */
void RenderArea::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)    // 放大
    {
    }
    else                      // 缩小
    {
    }

    QWidget::wheelEvent(event);
}
