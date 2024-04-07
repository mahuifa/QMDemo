#ifndef FORMULA_H
#define FORMULA_H
#include <QObject>


// 瓦片地图编号转经纬度
extern qreal tileTolon(int x, int z);
extern qreal tileTolat(int y, int z);

// 经纬度转瓦片地图编号
extern int lonTotile(qreal lon, int z);
extern int latTotile(qreal lat, int z);

// 经纬度转距离 Vincenty
extern qreal toDistance(qreal lon1, qreal lat1, qreal lon2, qreal lat2);
// 使用距离计算经纬度
extern qreal toLon(qreal lon, qreal lat, int dis);
extern qreal toLat(qreal lon, qreal lat, int dis);

// 计算同一瓦片中的纬度到上边界的像素差
extern qreal LatDisToPixel(qreal lon, qreal lat, int z);
// 计算一个经纬度点到距离dis外另一个点的像素差
extern qreal LatDisToPixel(qreal lon, qreal lat, int dis, int z);

// 计算web墨卡托投影瓦片地图像素分辨率(1像素：m)
extern qreal pixelToMeter(int z);    // 这种方式算的是赤道上的分辨率，越往两级误差越大
extern qreal pixelToMeter(int x, int y, int z);  // 精度更高的计算方式

// 合并两个int
extern qint64 MergeInt(qint64 v1, qint64 v2);
extern void msleep(int msec);
#endif // FORMULA_H
