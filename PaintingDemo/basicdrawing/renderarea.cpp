#include "renderarea.h"

#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QTextDocument>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    // 设置背景填充色
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);
}

void RenderArea::paintEvent(QPaintEvent *event)
{

}
