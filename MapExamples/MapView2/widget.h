#ifndef WIDGET_H
#define WIDGET_H

#include "mapStruct.h"
#include <QFuture>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
class Widget;
}

QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget() override;

private slots:
    void on_but_open_clicked();

private:
    QVector<int> getDir(const QString& path);
    QVector<int> getFile(const QString& path);

    void getTitles(int level);
    void loadImages();
    void zoom(bool flag);
    void showRect(QRect rect);
    void mousePos(QPoint pos);

private:
    Ui::Widget* ui;
    QVector<int> m_levels;
    int m_level = 0;   // 当前瓦片层级
    int m_index = 0;   // 索引
    QList<QPoint> m_tiles;
    QList<ImageInfo> m_imageInfos;   // 瓦片地图信息
    QFuture<void> m_future;
    QPoint m_centre;
};
#endif   // WIDGET_H
