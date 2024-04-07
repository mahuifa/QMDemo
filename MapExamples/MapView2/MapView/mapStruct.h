#ifndef MAPSTRUCT_H
#define MAPSTRUCT_H
/********************************************************************
 * 文件名： mapStruct.h
 * 时间：   2024-01-19 22:20:45
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   包含程序中使用到的结构体
 * ******************************************************************/

#include <QPixmap>
#include <QPointF>
#include <QString>

struct ImageInfo
{
    // 瓦片编号
    int x;
    int y;
    int z;
    QString url;       // 下载瓦片的地址
    QString format;    // 图片格式
    QPixmap img;       // 保存下载后的瓦片
    short count = 0;   // 失败下载次数，初始为0，下载失败一次+1
};

#endif   // MAPSTRUCT_H
