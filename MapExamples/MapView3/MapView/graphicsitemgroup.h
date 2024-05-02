#ifndef GRAPHICSITEMGROUP_H
#define GRAPHICSITEMGROUP_H

#include "mapStruct.h"
#include <QGraphicsItemGroup>

class GraphicsItemGroup : public QGraphicsItemGroup
{
public:
    explicit GraphicsItemGroup(QGraphicsItem* parent = nullptr);
    ~GraphicsItemGroup() override;

    void addImage(const ImageInfo& info);   // 添加瓦片图

private:
    QHash<quint64, QGraphicsPixmapItem*> m_itemsImg;        // 保存瓦片地图图元
    QHash<quint64, QGraphicsRectItem*> m_itemsR;            // 保存瓦片地图图元
    QHash<quint64, QGraphicsSimpleTextItem*> m_itemsText;   // 保存瓦片地图图元
};

#endif   // GRAPHICSITEMGROUP_H
