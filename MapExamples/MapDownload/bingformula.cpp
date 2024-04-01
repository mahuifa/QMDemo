/********************************************************************
 * 文件名： bingformula.cpp
 * 时间：   2024-04-05 21:36:16
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   适用于Bing瓦片地图的算法
 * ******************************************************************/
#include "bingformula.h"
#include <qstring.h>
#include <QtMath>

static const qreal g_EarthRadius = 6'378'137;   // 赤道半径

/**
 * @brief      限定最小值，最大值范围
 * @param n    需要限定的值
 * @param min
 * @param max
 * @return
 */
qreal Bing::clip(qreal n, qreal min, qreal max)
{
    n = qMax(n, min);
    n = qMin(n, max);
    return n;
}

/**
 * @brief      限定经度范围值，防止超限，经度范围[-180, 180]
 * @param lon  输入的经度
 * @return     裁剪后的经度
 */
qreal Bing::clipLon(qreal lon)
{
    return clip(lon, -180.0, 180);
}

/**
 * @brief      限定纬度范围值，防止超限，经度范围[-85.05112878, 85.05112878]
 * @param lat  输入的纬度
 * @return     裁剪后的纬度
 */
qreal Bing::clipLat(qreal lat)
{
    return clip(lat, -85.05112878, 85.05112878);
}

/**
 * @brief       根据输入的瓦片级别计算全地图总宽高，适用于墨卡托投影
 * @param level 1-23（bing地图没有0级别，最低级别为1，由4块瓦片组成）
 * @return      以像素为单位的地图宽度和高度。
 */
uint Bing::mapSize(int level)
{
    uint w = 256;   // 第0级别为256*256
    return (w << level);
}

/**
 * @brief        计算指定纬度、级别的地面分辨率（不同纬度分辨率不同）
 * @param lat    纬度
 * @param level  地图级别 1-23（bing地图没有0级别，最低级别为1，由4块瓦片组成）
 * @return       地面分辨率 单位（米/像素）
 */
qreal Bing::groundResolution(qreal lat, int level)
{
    lat = clipLat(lat);
    return qCos(lat * M_PI / 180) * 2 * M_PI * g_EarthRadius / mapSize(level);
}

/**
 * @brief           计算地图比例尺，地面分辨率和地图比例尺也随纬度而变化
 * @param lat       纬度
 * @param level     地图级别 1-23（bing地图没有0级别，最低级别为1，由4块瓦片组成）
 * @param screenDpi 屏幕分辨率，单位为点/英寸  通常为 96 dpi
 * @return          地图比例尺 1:N（地图上1厘米表示实际N厘米）
 */
qreal Bing::mapScale(qreal lat, int level, int screenDpi)
{
    return groundResolution(lat, level) * screenDpi / 0.0254;   // 1英寸等于0.0254米
}

/**
 * @brief         将一个点从纬度/经度WGS-84墨卡托坐标(以度为单位)转换为指定细节级别的像素XY坐标。
 * @param lon     经度
 * @param lat     纬度
 * @param level   地图级别
 * @return        像素坐标
 */
QPoint Bing::latLongToPixelXY(qreal lon, qreal lat, int level)
{
    lon = clipLon(lon);
    lat = clipLat(lat);

    qreal x = (lon + 180) / 360;
    qreal sinLat = qSin(lat * M_PI / 180);
    qreal y = 0.5 - qLn((1 + sinLat) / (1 - sinLat)) / (4 * M_PI);

    uint size = mapSize(level);
    qreal pixelX = x * size + 0.5;
    pixelX = clip(pixelX, 0, size - 1);
    qreal pixelY = y * size + 0.5;
    pixelY = clip(pixelY, 0, size - 1);

    return QPoint(pixelX, pixelY);
}

/**
 * @brief         将像素从指定细节级别的像素XY坐标转换为经纬度WGS-84坐标(以度为单位)
 * @param pos    像素坐标
 * @param level
 * @param lon
 * @param lat
 */
void Bing::pixelXYToLatLong(QPoint pos, int level, qreal& lon, qreal& lat)
{
    uint size = mapSize(level);
    qreal x = (clip(pos.x(), 0, size - 1) / size) - 0.5;
    qreal y = 0.5 - (clip(pos.y(), 0, size - 1) / size);
    lon = x * 360;
    lat = 90 - (360 * qAtan(qExp(-y * 2 * M_PI)) / M_PI);
}

/**
 * @brief     像素坐标转瓦片编号
 * @param pos  像素坐标
 * @return    瓦片编号
 */
QPoint Bing::pixelXYToTileXY(QPoint pos)
{
    int x = pos.x() / 256;
    int y = pos.y() / 256;
    return QPoint(x, y);
}

/**
 * @brief       瓦片编号转像素坐标
 * @param tile  瓦片编号
 * @return      像素坐标
 */
QPoint Bing::tileXYToPixelXY(QPoint tile)
{
    int x = tile.x() * 256;
    int y = tile.y() * 256;
    return QPoint(x, y);
}

/**
 * @brief       经纬度转瓦片编号
 * @param lon
 * @param lat
 * @param level
 * @return
 */
QPoint Bing::latLongToTileXY(qreal lon, qreal lat, int level)
{
    return pixelXYToTileXY(latLongToPixelXY(lon, lat, level));
}

/**
 * @brief         瓦片编号转经纬度
 * @param tile
 * @param level
 * @return       经纬度 x:经度  y纬度
 */
QPointF Bing::tileXYToLatLong(QPoint tile, int level)
{
    qreal lon = 0;
    qreal lat = 0;
    QPoint pos = tileXYToPixelXY(tile);
    pixelXYToLatLong(pos, level, lon, lat);
    return QPointF(lon, lat);
}

/**
 * @brief         瓦片编号转 bing请求的QuadKey
 * @param tile   瓦片编号
 * @param level  瓦片级别
 * @return
 */
QString Bing::tileXYToQuadKey(QPoint tile, int level)
{
    QString key;
    for (int i = level; i > 0; i--)
    {
        char digit = '0';
        int mask = 1 << (i - 1);
        if ((tile.x() & mask) != 0)
        {
            digit++;
        }
        if ((tile.y() & mask) != 0)
        {
            digit += 2;
        }
        key.append(digit);
    }
    return key;
}

/**
 * @brief            将一个QuadKey转换为瓦片XY坐标。
 * @param quadKey
 * @param tileX      返回瓦片X编号
 * @param tileY      返回瓦片Y编号
 * @param level      返回瓦片等级
 */
void Bing::quadKeyToTileXY(QString quadKey, int& tileX, int& tileY, int& level)
{
    tileX = 0;
    tileY = 0;
    level = quadKey.count();
    QByteArray buf = quadKey.toUtf8();
    for (int i = level; i > 0; i--)
    {
        int mask = 1 << (i - 1);
        switch (buf.at(i - 1))
        {
        case '0':
            break;
        case '1':
            tileX |= mask;
            break;
        case '2':
            tileY |= mask;
            break;
        case '3':
            tileX |= mask;
            tileY |= mask;
            break;
        default:
            break;
        }
    }
}
