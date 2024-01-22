#ifndef DOWNLOADINFO_H
#define DOWNLOADINFO_H
#include "mapStruct.h"

// ArcGis瓦片下载信息计算
void getArcGisMapInfo(const MapInfo& mapInfo, QList<ImageInfo>& infos);

#endif // DOWNLOADINFO_H
