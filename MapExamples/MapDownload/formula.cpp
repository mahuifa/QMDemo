/********************************************************************
 * 文件名： formula.cpp
 * 时间：   2024-04-05 21:34:39
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   瓦片地图中坐标转换算法，适用于谷歌、高德、ArcGis等
 * ******************************************************************/
#include "formula.h"
#include <QtMath>
#include <QDebug>
#include <qeventloop.h>
#include <qtimer.h>

/**
 * @brief    x转经度    经度 = (x / 2^z) * 360 - 180
 * @param x
 * @param z
 * @return
 */
qreal tileTolon(int x, int z)
{
    return (x / qPow(2, z) * 360 - 180);
}

/**
 * @brief     编号y转纬度  纬度 = arctan((sinh(π * (1 - 2y/2^z))) * tan(π/4 + πy/2^z))
 * @param y
 * @param z
 * @return
 */
qreal tileTolat(int y, int z)
{
    qreal n= M_PI - 2 * M_PI * y / qPow(2 , z);
    return (180 / M_PI * qAtan(0.5*(qExp(n) - qExp(-n))));
}

/**
 * @brief        经度转编号x
 * @param lon   [-180, 180)
 * @param z
 * @return
 */
int lonTotile(qreal lon, int z)
{
    // 限定经度范围
    if(lon < -180)
    {
        lon = -180;
    }
    if(lon >= 180)
    {
        lon = 179.9999999;
    }
    return (qFloor((lon + 180) / 360 * qPow(2, z)));
}

/**
 * @brief       纬度转瓦片地图编号Y
 * @param lat  [-85.051128, 85.051128]
 * @param z
 * @return
 */
int latTotile(qreal lat, int z)
{
    // 限定纬度范围
    if (lat > 85.051128)
    {
        lat = 85.051128;
    }
    if (lat < -85.051128)
    {
        lat = -85.051128;
    }
    return (qFloor((1 - qLn(qTan(lat * M_PI / 180) + 1 / qCos(lat * M_PI / 180)) / M_PI) / 2 * qPow(2, z)));
}

// 地球平均半径
static const qreal g_r = 6371008;
/**
 * @brief       将两点经纬度 转为距离(Vincenty公式)
 * @param lon1  经度1
 * @param lat1  纬度1
 * @param lon2
 * @param lat2
 * @return
 */
qreal toDistance(qreal lon1, qreal lat1, qreal lon2, qreal lat2)
{
    qreal deltaLatitude = qDegreesToRadians(lat2 - lat1);
    qreal deltaLongitude = qDegreesToRadians(lon2 - lon1);
    lat1 = qDegreesToRadians(lat1);
    lat2 = qDegreesToRadians(lat2);
    qreal a = pow(sin(deltaLatitude / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(deltaLongitude / 2), 2);
    qreal c = 2 * atan2(sqrt(a), sqrt(1 - a));

    qreal d = g_r * c;
    return d;
}

/**
 * @brief   计算web墨卡托投影瓦片地图像素分辨率
 * @param z 缩放级别
 * @return  1像素:xxx米
 */
qreal pixelToMeter(int z)
{
    if(z < 0) return 0;
    // 赤道半径为6378137，Web墨卡托瓦片地图0级别只有一张256*256
    qreal b = 6378137 * M_PI * 2 / 256;
    return b / qPow(2, z);
}

qreal pixelToMeter(int x, int y, int z)
{
    qreal lon1 = tileTolon(x, z);
    qreal lon2 = tileTolon(x + 1, z);
    qreal lat1 = tileTolon(y, z);
    qreal dis = toDistance(lon1, lat1, lon2, lat1);  // 通过相邻两个瓦片左上角经纬度计算瓦片边长的距离
    qreal meter = dis / 256;

    return meter;
}

qint64 MergeInt(qint64 v1, qint64 v2)
{
    QString str = QString("%1%2").arg(v1).arg(v2);
    return  str.toLongLong();
}

// 经度相同时，1米等于0.00000899纬度
static const qreal g_latRatio=  0.00000899;
// 纬度相同时，1米等于0.00001141经度
static const qreal g_lonRatio=  0.00001141;
/**
 * @brief      使用距离、当前位置经纬度计算 距离差上的纬度距离（南北纬度平均值）
 * @param lon  当前经纬度
 * @param lat
 * @param dis  距离
 * @return     返回纬度差值
 */
qreal toLat(qreal lon, qreal lat, int dis)
{
    int tempDis = 0;
    qreal tempLat = lat + (dis * g_latRatio);   // 计算一定距离上的纬度值
    // 通过将纬度值使用Vincenty公式计算为距离，在反算校准误差
    for(int i = 0; i <10; i++)   // 如果10次还不能纠正误差，则退出循环，防止死循环
    {
        tempDis = static_cast<int>(toDistance(lon, lat, lon, tempLat));
        int diff = tempDis - dis ;

        if(qAbs(diff) < 3)    // 距离误差3m以内
        {
            break;
        }
        else
        {
            tempLat -= (diff * g_latRatio);
        }
    }
    return tempLat - lat;
}

/**
 * @brief       使用距离、当前位置经纬度计算经度距离（南北纬度平均值）
 * @param lon   当前经纬
 * @param lat
 * @param dis  距离
 * @return     返回经度差值
 */
qreal toLon(qreal lon, qreal lat, int dis)
{
    int tempDis = 0;
    qreal tempLon = lon + (dis * g_lonRatio);   // 计算一定距离上的经度值
    // 通过将经度值使用Vincenty公式计算为距离，在反算校准误差
    for(int i = 0; i < 10; i++)   // 如果10次还不能纠正误差，则退出循环，防止死循环
    {
        tempDis = static_cast<int>(toDistance(lon, lat, tempLon, lat));
        int diff = tempDis - dis ;

        if(qAbs(diff) < 3)    // 距离误差3m以内
        {
            break;
        }
        else
        {
            tempLon -= (diff * g_lonRatio);
        }
    }
    return tempLon - lon;
}

/**
 * @brief      计算同一瓦片中的纬度到上边界的像素差
 * @param lon
 * @param lat
 * @param z
 * @return
 */
qreal LatDisToPixel(qreal lon, qreal lat, int z)
{
    int x = lonTotile(lon, z);       // 计算瓦片编号
    int y = latTotile(lat, z);       // 计算瓦片编号
    qreal yLat = tileTolat(y, z);    // 计算瓦片上边纬度
    qreal ratio = pixelToMeter(x, y, z);  // 计算瓦片中像素分辨率
    qreal disY = toDistance(lon, lat, lon, yLat);  // 经度相同时计算纬度上的距离
    qreal pixelY = disY / ratio;    // 计算纬度上像素差

    return  pixelY;
}

/**
 * @brief      通过瓦片地图中的经纬度计算瓦片编号，然后计算瓦片上边的纬度，在计算输入纬度到上边纬度的像素
 *             TODO：还需要考虑南纬
 * @param lon
 * @param lat
 * @param dis
 * @param z
 * @return      瓦片中经纬度到瓦片上边的像素差
 */
qreal LatDisToPixel(qreal lon, qreal lat, int dis, int z)
{
    // 计算中心瓦片编号
    int cY = latTotile(lat, z);
    qreal cPixel = LatDisToPixel(lon, lat, z);  // 中心瓦片经纬度到上边的像素差

    qreal tempLat = toLat(lon, lat, dis);   // 计算纬度差
    qreal topLat = lat + tempLat;
    qreal pixel = LatDisToPixel(lon, topLat, z);  // 距离外的瓦片经纬度到上边的像素差

    int dY = latTotile(topLat, z);
    int idDiff = cY - dY;
    qreal pixelSun = 0;
    if(idDiff)
    {
        pixelSun = (idDiff - 1) * 256 + cPixel + (255 - pixel);
    }
    else  // 同一张瓦片中
    {
        pixelSun = cPixel - pixel;
    }

    return pixelSun;
}

/**
 * @brief       非阻塞毫秒延时
 * @param msec
 */
void msleep(int msec)
{
    if(msec <= 0)return;

    QEventLoop loop;
    QTimer::singleShot(msec, &loop, &QEventLoop::quit);
    loop.exec();
}
