/********************************************************************
 * 文件名： mapinput.cpp
 * 时间：   2024-01-19 22:22:37
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   生成地图下载的输入信息
 * ******************************************************************/
#include "mapinput.h"
#include "ui_mapinput.h"
#include "downloadinfo.h"
#include <QDebug>

MapInput::MapInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapInput)
{
    ui->setupUi(this);

    // 默认下载范围
    ui->dspin_LTLon->setValue(103.979482);
    ui->dspin_LTLat->setValue(30.66596);
    ui->dspin_RDLon->setValue(104.137009);
    ui->dspin_RDLat->setValue(30.554068);
    initArcGis();
}

MapInput::~MapInput()
{
    delete ui;
}

/**
 * @brief 填入ArcGis下载地图类型
 */
void MapInput::initArcGis()
{
    ui->com_type->addItem("NatGeo_World_Map");
    ui->com_type->addItem("USA_Topo_Maps ");
    ui->com_type->addItem("World_Imagery");
    ui->com_type->addItem("World_Physical_Map");
    ui->com_type->addItem("World_Shaded_Relief");
    ui->com_type->addItem("World_Street_Map");
    ui->com_type->addItem("World_Terrain_Base");
    ui->com_type->addItem("World_Topo_Map");
    ui->com_type->addItem("Canvas/World_Dark_Gray_Base");
    ui->com_type->addItem("Canvas/World_Dark_Gray_Reference");
    ui->com_type->addItem("Canvas/World_Light_Gray_Base");
    ui->com_type->addItem("Canvas/World_Light_Gray_Reference");
    ui->com_type->addItem("Elevation/World_Hillshade_Dark");
    ui->com_type->addItem("Elevation/World_Hillshade");
    ui->com_type->addItem("Ocean/World_Ocean_Base");
    ui->com_type->addItem("Ocean/World_Ocean_Reference");
    ui->com_type->addItem("Polar/Antarctic_Imagery");
    ui->com_type->addItem("Polar/Arctic_Imagery");
    ui->com_type->addItem("Polar/Arctic_Ocean_Base");
    ui->com_type->addItem("Polar/Arctic_Ocean_Reference");
    ui->com_type->addItem("Reference/World_Boundaries_and_Places_Alternate ");
    ui->com_type->addItem("Reference/World_Boundaries_and_Places");
    ui->com_type->addItem("Reference/World_Reference_Overlay");
    ui->com_type->addItem("Reference/World_Transportation");
    ui->com_type->addItem("Specialty/World_Navigation_Charts");

    // 填入下载格式
    ui->com_format->addItem("jpg");
    ui->com_format->addItem("png");
    ui->com_format->addItem("bmp");
}


/**
 * @brief   计算并返回需要下载的瓦片地图信息
 * @return
 */
const QList<ImageInfo> &MapInput::getInputInfo()
{
    m_infos.clear();    // 清除之前的内容

    MapInfo info;
    info.topLeft = QPointF(ui->dspin_LTLon->value(), ui->dspin_LTLat->value());
    info.lowRight = QPointF(ui->dspin_RDLon->value(), ui->dspin_RDLat->value());
    info.z = ui->spin_z->value();
    info.type = ui->com_type->currentText();
    info.format = ui->com_format->currentText();

    switch (ui->tabWidget->currentIndex())   // 判断是什么类型的地图源
    {
    case 0:     // ArcGis
    {
        getArcGisMapInfo(info, m_infos);      // 计算下载信息
        break;
    }
    case 1:
    {
        break;
    }
    case 2:
    {
        break;
    }
    default:
        break;
    }
    qDebug() << "瓦片数：" << m_infos.count();
    return m_infos;
}
