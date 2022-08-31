#ifndef QGRAPHICSPOINTSITEM_H
#define QGRAPHICSPOINTSITEM_H

#include <QGraphicsItem>
#include <qpen.h>

class QPainterPath;
class QPainter;
class QRectF;

class QGraphicsPointsItem : public QGraphicsItem
{
public:
    explicit QGraphicsPointsItem(QGraphicsItem *parent = nullptr);
    ~QGraphicsPointsItem();

    QPen pen() const;
    void setPen(const QPen &pen);

    QPolygonF points() const;
    void setPoints(const QPolygonF& points);


    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    enum { Type = 6 };
    int type() const override;

private:
    QPen m_pen;
    QPolygonF m_points;
};

#endif // QGRAPHICSPOINTSITEM_H
