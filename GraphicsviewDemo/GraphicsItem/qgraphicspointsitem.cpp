#include "qgraphicspointsitem.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


QGraphicsPointsItem::QGraphicsPointsItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{

}

QGraphicsPointsItem::~QGraphicsPointsItem()
{

}

QPen QGraphicsPointsItem::pen() const
{
    return m_pen;
}

void QGraphicsPointsItem::setPen(const QPen &pen)
{
    m_pen = pen;
}

QPolygonF QGraphicsPointsItem::points() const
{
    return m_points;
}

void QGraphicsPointsItem::setPoints(const QPolygonF &points)
{
    m_points = points;
}

QRectF QGraphicsPointsItem::boundingRect() const
{
    return m_points.boundingRect();
}

/**
 * @brief          从源码复制过来的内部函数，主要是负责绘制选中图元时的边框样式
 * @param item
 * @param painter
 * @param option
 */
static void qt_graphicsItem_highlightSelected(
    QGraphicsItem *item, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    const QRectF murect = painter->transform().mapRect(QRectF(0, 0, 1, 1));
    if (qFuzzyIsNull(qMax(murect.width(), murect.height())))
        return;

    const QRectF mbrect = painter->transform().mapRect(item->boundingRect());
    if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
        return;

    const qreal pad = static_cast<QGraphicsPointsItem *>(item)->pen().widthF() / 2;

    const qreal penWidth = 0; // cosmetic pen

    const QColor fgcolor = option->palette.windowText().color();
    const QColor bgcolor( // ensure good contrast against fgcolor
        fgcolor.red()   > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue()  > 127 ? 0 : 255);

    painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(item->boundingRect().adjusted(pad, pad, -pad, -pad));

    painter->setPen(QPen(option->palette.windowText(), 0, Qt::DashLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(item->boundingRect().adjusted(pad, pad, -pad, -pad));
}

void QGraphicsPointsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    painter->setPen(m_pen);
    painter->drawPoints(m_points);

    // 绘制选择图元时的边框样式（也可以不用）
    if (option->state & QStyle::State_Selected)
        qt_graphicsItem_highlightSelected(this, painter, option);
}

int QGraphicsPointsItem::type() const
{
    return type();
}
