#ifndef DOWNLOADINFO_H
#define DOWNLOADINFO_H

#include <QPointF>
#include <QString>

struct MapInfo
{
    QPointF topLeft;        // 左上角经纬度
    QPointF lowRight;       // 右下角经纬度
    int z;                  // 下载层级
};

struct ImageInfo
{
    // 瓦片编号
    int x;
    int y;
    int z;
    QString url;         // 下载瓦片的地址
};

bool getMapInfo(const MapInfo& mapInfo, QList<ImageInfo>& infos);

#endif // DOWNLOADINFO_H
