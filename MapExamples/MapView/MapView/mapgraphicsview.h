#ifndef MAPGRAPHICSVIEW_H
#define MAPGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class MapGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapGraphicsView(QWidget *parent = nullptr);
    ~MapGraphicsView();

    void setPath(const QString& path);

protected:
    void wheelEvent(QWheelEvent *event) override;

signals:
    void addImage(QPixmap img, QPoint pos);
private:
    void getMapLevel();     // 获取路径中瓦片地图的层级
    void getTitle();        // 获取路径中瓦片地图编号
    void loatImage();       // 加载瓦片
    void clearMapItem();    // 清除显示瓦片
    void clearHash();       // 清除瓦片图元哈希表
    int getKey();          // 获取当前显示的层级key值
    void on_addImage(QPixmap img, QPoint pos);

private:
    QGraphicsScene* m_scene = nullptr;
    QString m_path;          // 瓦片地图文件路径
    QHash<int, QGraphicsItemGroup*> m_mapItemGroup; // 存放地图图元组的数组，以瓦片层级为key
    int m_keyIndex = 0;               // 当前显示的瓦片层级
    QVector<QPoint> m_imgTitle;       // 保存图片编号
    QGraphicsItemGroup* m_itemGroup = nullptr;  // 当前显示层级图元
};

#endif // MAPGRAPHICSVIEW_H
