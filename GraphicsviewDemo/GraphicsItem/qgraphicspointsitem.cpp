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

/**
 * @brief  返回绘制的画笔对象
 * @return
 */
QPen QGraphicsPointsItem::pen() const
{
    return m_pen;
}

/**
 * @brief       设置画笔对象
 * @param pen
 */
void QGraphicsPointsItem::setPen(const QPen &pen)
{
    if (m_pen == pen)
        return;
    prepareGeometryChange();          // 准备几何图形更改,以使QGraphicsScene的索引保持最新
    m_pen = pen;
    update();
}

/**
 * @brief   返回用于绘制的点数据
 * @return
 */
QPolygonF QGraphicsPointsItem::points() const
{
    return m_points;
}

/**
 * @brief         设置用于绘制的散点数据
 * @param points
 */
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

/**
 * @brief        判断是否包含传入的点
 * @param point  传入数据点
 * @return       包含返回true，否则返回false
 */
bool QGraphicsPointsItem::contains(const QPointF &point) const
{
#if 1      // 用这种方法只有QPolygonF中有的点才返回true
    for(auto pointf : m_points)
    {
        if(pointf == point)
        {
            return true;
        }
    }
    return false;
#else      // 用这种方法只要包含在当前图元边框矩形内的点都返回true
    return QGraphicsItem::contains(point);
#endif
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

/**
 * @brief            重写这个函数进行自定义图案绘制
 * @param painter    用于绘制的画家对象
 * @param option     QStyleOptionsGraphicsItem类用于描述绘制QGraphicsItem所需的参数。
 * @param widget
 */
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
