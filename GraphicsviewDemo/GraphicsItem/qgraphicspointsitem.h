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
    ~QGraphicsPointsItem() override;

    QPen pen() const;
    void setPen(const QPen &pen);

    QPolygonF points() const;
    void setPoints(const QPolygonF& points);


    QRectF boundingRect() const override;
    bool contains(const QPointF &point) const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    bool isObscuredBy(const QGraphicsItem *item) const override;
    QPainterPath opaqueArea() const override;
    enum { Type = 15 };
    int type() const override;

protected:
    bool supportsExtension(Extension extension) const override;
    void setExtension(Extension extension, const QVariant &variant) override;
    QVariant extension(const QVariant &variant) const override;

private:
    QPen m_pen;
    QPolygonF m_points;
};

#endif // QGRAPHICSPOINTSITEM_H
