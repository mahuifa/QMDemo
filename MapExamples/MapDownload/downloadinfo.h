#ifndef DOWNLOADINFO_H
#define DOWNLOADINFO_H

#include <QImage>
#include <QPointF>
#include <QString>

struct MapInfo
{
    QPointF topLeft;        // 左上角经纬度
    QPointF lowRight;       // 右下角经纬度
    int z;                  // 下载层级
    QString type;           // 地图类型
    QString format;         // 图片格式
};

struct ImageInfo
{
    // 瓦片编号
    int x;
    int y;
    int z;
    QString url;          // 下载瓦片的地址
    QImage img;           // 保存下载后的瓦片
    short count = 0;      // 失败下载次数，初始为0，下载失败一次+1
};

// ArcGis瓦片下载信息计算
void getArcGisMapInfo(const MapInfo& mapInfo, QList<ImageInfo>& infos);

#endif // DOWNLOADINFO_H
