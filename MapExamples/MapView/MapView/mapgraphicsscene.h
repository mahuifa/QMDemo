#ifndef MAPGRAPHICSSCENE_H
#define MAPGRAPHICSSCENE_H

#include <QGraphicsScene>

class MapGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapGraphicsScene(QObject *parent = nullptr);
    ~MapGraphicsScene();

signals:

};

#endif // MAPGRAPHICSSCENE_H
