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
    if (m_pen == pen)
        return;
    prepareGeometryChange();
    m_pen = pen;
    update();
}

QPolygonF QGraphicsPointsItem::points() const
{
    return m_points;
}

void QGraphicsPointsItem::setPoints(const QPolygonF &points)
{
    if (m_points == points)
        return;
    prepareGeometryChange();
    m_points = points;
    update();
}

/**
 * @brief   返回本图元的边界矩形，所有绘制都必须放在这个矩形内部，
 *          这个矩形决定了重绘区域，如果绘制内容超过这个矩形则在移动图元重绘时会【产生残留】
 * @return
 */
QRectF QGraphicsPointsItem::boundingRect() const
{
    const qreal pad = m_pen.widthF() / 2;
    return m_points.boundingRect().adjusted(-pad, -pad, pad, pad);
}


bool QGraphicsPointsItem::contains(const QPointF &point) const
{
    return QGraphicsItem::contains(point);
}

/**
 * @brief          从源码复制过来并修改的内部函数，主要是负责绘制选中图元时的边框样式
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

    const QColor fgcolor = option->palette.windowText().color();
    const QColor bgcolor( // ensure good contrast against fgcolor
        fgcolor.red()   > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue()  > 127 ? 0 : 255);


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

bool QGraphicsPointsItem::isObscuredBy(const QGraphicsItem *item) const
{
    return QGraphicsItem::isObscuredBy(item);
}

/**
 * @brief      没啥用，但是默认的图元类中这样写就留着
 * @return
 */
QPainterPath QGraphicsPointsItem::opaqueArea() const
{
    return QGraphicsItem::opaqueArea();
}

int QGraphicsPointsItem::type() const
{
    return Type;
}

/**
 * @brief           没啥用，但是默认的图元类中这样写就留着
 * @param extension
 * @return
 */
bool QGraphicsPointsItem::supportsExtension(QGraphicsItem::Extension extension) const
{
    Q_UNUSED(extension)
    return false;
}

/**
 * @brief             没啥用，但是默认的图元类中这样写就留着
 * @param extension
 * @param variant
 */
void QGraphicsPointsItem::setExtension(QGraphicsItem::Extension extension, const QVariant &variant)
{
    Q_UNUSED(extension)
    Q_UNUSED(variant)
}

/**
 * @brief           没啥用，但是默认的图元类中这样写就留着
 * @param variant
 * @return
 */
QVariant QGraphicsPointsItem::extension(const QVariant &variant) const
{
    Q_UNUSED(variant)
    return QVariant();
}
