#include "renderarea.h"

#include <qpainter.h>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    initPath();
}


void RenderArea::initPath()
{

}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
}
