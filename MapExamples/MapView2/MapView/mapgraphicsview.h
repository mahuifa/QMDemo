#ifndef MAPGRAPHICSVIEW_H
#define MAPGRAPHICSVIEW_H

#include "mapStruct.h"
#include <QGraphicsView>

class MapGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapGraphicsView(QWidget* parent = nullptr);
    ~MapGraphicsView() override;

    void setRect(QRect rect);
    void drawImg(const ImageInfo& info);
    void clear();

signals:
    void updateImage(const ImageInfo& info);   // 添加瓦片图
    void zoom(bool flag);                      // 缩放 true：放大
    void showRect(QRect rect);
    void mousePos(QPoint pos);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void getShowRect();   // 获取显示范围

private:
    QGraphicsScene* m_scene = nullptr;
    QPointF m_pos;
    QPointF m_scenePos;
};

#endif   // MAPGRAPHICSVIEW_H
