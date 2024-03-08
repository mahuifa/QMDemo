#ifndef MAPINPUT_H
#define MAPINPUT_H

#include <QWidget>
#include "mapStruct.h"

namespace Ui {
class MapInput;
}

class MapInput : public QWidget
{
    Q_OBJECT

public:
    explicit MapInput(QWidget *parent = nullptr);
    ~MapInput();

    const QList<ImageInfo> &getInputInfo();       // 获取下载地图所需的输入信息

private:
    // ArcGis
    void initArcGis();
    void getArcGisMapInfo();
    // 高德
    void initAMap();
    void getAMapInfo();
    // Bing地图
    void initBing();
    void getBingMapInfo();

private:
    Ui::MapInput *ui;
    QList<ImageInfo> m_infos;                // 保存下载瓦片图片的信息
};

#endif // MAPINPUT_H
