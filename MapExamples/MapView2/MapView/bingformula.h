#ifndef BINGFORMULA_H
#define BINGFORMULA_H
#include <QPoint>
#include <QtGlobal>

namespace Bing {
qreal clip(qreal n, qreal min, qreal max);
qreal clipLon(qreal lon);   // 裁剪经度范围
qreal clipLat(qreal lat);   // 裁剪纬度范围

uint mapSize(int level);                        // 根据地图级别计算世界地图总宽高(以像素为单位)
qreal groundResolution(qreal lat, int level);   // 计算地面分辨率
qreal mapScale(qreal lat, int level, int screenDpi);   // 计算比例尺

QPoint latLongToPixelXY(qreal lon, qreal lat, int level);               // 经纬度转像素 XY坐标
void pixelXYToLatLong(QPoint pos, int level, qreal& lon, qreal& lat);   // 像素坐标转WGS-84墨卡托坐标

QPoint pixelXYToTileXY(QPoint pos);    // 像素坐标转瓦片编号
QPoint tileXYToPixelXY(QPoint tile);   // 瓦片编号转像素坐标

QPoint latLongToTileXY(qreal lon, qreal lat, int level);   // 经纬度转瓦片编号
QPointF tileXYToLatLong(QPoint tile, int level);           // 瓦片编号转经纬度

QString tileXYToQuadKey(QPoint tile, int level);                             // 瓦片编号转QuadKey
void quadKeyToTileXY(QString quadKey, int& tileX, int& tileY, int& level);   // QuadKey转瓦片编号、级别
}   // namespace Bing
#endif   // BINGFORMULA_H
