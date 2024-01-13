/********************************************************************
 * 文件名： downloadinfo.cpp
 * 时间：   2024-03-29 20:14:44
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   计算下载瓦片地图的信息
 * ******************************************************************/
#include "downloadinfo.h"
#include "formula.h"
#include <QDebug>
/**
 * @brief          通过输入地图信息计算需要下载的瓦片图信息，下载ArcGIS地图，WGS84坐标系，Web墨卡托投影，z y x输入
 * @param mapInfo  输入需要下载的信息
 * @param infos    输出计算后的信息
 */
void getArcGisMapInfo(const MapInfo &mapInfo, QList<ImageInfo> &infos)
{
    static QString url = "https://server.arcgisonline.com/arcgis/rest/services/%1/MapServer/tile/%2/%3/%4.%5";
    int ltX = lonTotile(mapInfo.topLeft.x(), mapInfo.z);   // 计算左上角瓦片X
    int ltY = latTotile(mapInfo.topLeft.y(), mapInfo.z);   // 计算左上角瓦片Y
    int rdX = lonTotile(mapInfo.lowRight.x(), mapInfo.z);   // 计算右下角瓦片X
    int rdY = latTotile(mapInfo.lowRight.y(), mapInfo.z);   // 计算右下角瓦片Y

    infos.clear();   // 清除已有的数据
    ImageInfo info;
    info.z = mapInfo.z;
    for(int x = ltX; x <= rdX; x++)
    {
        info.x = x;
        for(int y = ltY; y <= rdY; y++)
        {
            info.y = y;
            info.url = url.arg(mapInfo.type).arg(mapInfo.z).arg(y).arg(x).arg(mapInfo.format);
            infos.append(info);
            qDebug() << info.url;
        }
    }
}
