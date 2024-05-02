/********************************************************************
 * 文件名： mapinput.cpp
 * 时间：   2024-01-19 22:22:37
 * 开发者：  mhf
 * 邮箱：   1603291350@qq.com
 * 说明：   生成地图下载的输入信息
 * ******************************************************************/
#include "mapinput.h"
#include "bingformula.h"
#include "formula.h"
#include "ui_mapinput.h"
#include <QDebug>

MapInput::MapInput(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MapInput)
{
    ui->setupUi(this);

    initArcGis();
    initAMap();
    initBing();
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
    for (int i = 0; i < 23; i++)
    {
        ui->com_z->addItem(QString("%1").arg(i), i);
    }
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
const QList<ImageInfo>& MapInput::getInputInfo()
{
    m_infos.clear();   // 清除之前的内容

    switch (ui->tabWidget->currentIndex())   // 判断是什么类型的地图源
    {
    case 0:   // ArcGis
        {
            getArcGisMapInfo();   // 计算ArcGis下载信息
            break;
        }
    case 1:
        {
            getAMapInfo();   // 计算高德地图下载信息
            break;
        }
    case 2:
        {
            getBingMapInfo();   // 计算bing地图下载信息
            break;
        }
    default:
        break;
    }

    qDebug() << "瓦片数：" << m_infos.count();

    return m_infos;
}

/**
 * @brief   通过输入地图信息计算需要下载的瓦片图信息，下载ArcGIS地图，WGS84坐标系，Web墨卡托投影，z y x输入
 */
void MapInput::getArcGisMapInfo()
{
    static QString url = "https://server.arcgisonline.com/arcgis/rest/services/%1/MapServer/tile/%2/%3/%4.%5";

    int z = ui->com_z->currentData().toInt();
    QString type = ui->com_type->currentText();
    QString format = ui->com_format->currentText();
    QStringList lt = ui->line_LTGps->text().trimmed().split(',');   // 左上角经纬度
    QStringList rd = ui->line_RDGps->text().trimmed().split(',');   // 右下角经纬度
    if (lt.count() != 2 || rd.count() != 2)
        return;                                    // 判断输入是否正确
    int ltX = lonTotile(lt.at(0).toDouble(), z);   // 计算左上角瓦片X
    int ltY = latTotile(lt.at(1).toDouble(), z);   // 计算左上角瓦片Y
    int rdX = lonTotile(rd.at(0).toDouble(), z);   // 计算右下角瓦片X
    int rdY = latTotile(rd.at(1).toDouble(), z);   // 计算右下角瓦片Y

    ImageInfo info;
    info.z = z;
    info.format = format;
    for (int x = ltX; x <= rdX; x++)
    {
        info.x = x;
        for (int y = ltY; y <= rdY; y++)
        {
            info.y = y;
            info.url = url.arg(type).arg(z).arg(y).arg(x).arg(format);
            m_infos.append(info);
        }
    }
}

/**
 * @brief 初始化高德地图下载选项信息
 */
void MapInput::initAMap()
{
    for (int i = 1; i < 5; i++)
    {
        ui->com_amapPrefix->addItem(QString("wprd0%1").arg(i));
    }
    for (int i = 1; i < 5; i++)
    {
        ui->com_amapPrefix->addItem(QString("webst0%1").arg(i));
    }
    for (int i = 0; i < 19; i++)
    {
        ui->com_amapZ->addItem(QString("%1").arg(i), i);
    }
    // 语言设置
    ui->com_amapLang->addItem("中文", "zh_cn");
    ui->com_amapLang->addItem("英文", "en");
    // 地图类型
    ui->com_amapStyle->addItem("卫星影像图", 6);
    ui->com_amapStyle->addItem("矢量路网", 7);
    ui->com_amapStyle->addItem("影像路网", 8);        // 支持png透明背景
    ui->com_amapStyle->addItem("卫星+影像路网", 9);   // 支持png透明背景
    // 图片尺寸，只在7 8生效
    ui->com_amapScl->addItem("256x256", 1);
    ui->com_amapScl->addItem("512x512", 2);

    // 填入下载格式
    ui->com_amapFormat->addItem("jpg");
    ui->com_amapFormat->addItem("png");
    ui->com_amapFormat->addItem("bmp");
}

/**
 * @brief 计算高德地图瓦片下载信息
 */
void MapInput::getAMapInfo()
{
    static QString url = "https://%1.is.autonavi.com/appmaptile?";

    int z = ui->com_amapZ->currentData().toInt();
    QString format = ui->com_amapFormat->currentText();
    QStringList lt = ui->line_LTGps->text().trimmed().split(',');   // 左上角经纬度
    QStringList rd = ui->line_RDGps->text().trimmed().split(',');   // 右下角经纬度
    if (lt.count() != 2 || rd.count() != 2)
        return;                                    // 判断输入是否正确
    int ltX = lonTotile(lt.at(0).toDouble(), z);   // 计算左上角瓦片X
    int ltY = latTotile(lt.at(1).toDouble(), z);   // 计算左上角瓦片Y
    int rdX = lonTotile(rd.at(0).toDouble(), z);   // 计算右下角瓦片X
    int rdY = latTotile(rd.at(1).toDouble(), z);   // 计算右下角瓦片Y

    ImageInfo info;
    info.z = z;
    info.format = format;
    int style = ui->com_amapStyle->currentData().toInt();
    int count = 1;
    if (style == 9)
    {
        count = 2;   // 如果是下载卫星图 + 路网图则循环两次
    }

    for (int i = 0; i < count; i++)
    {
        if (count == 2)
        {
            if (i == 0)
            {
                style = 6;   // 第一次下载卫星图
                info.format = "jpg";
            }
            else
            {
                style = 8;             // 第二次下载路网图
                info.format = "png";   // 如果同时下载卫星图和路网图则路网图为透明png格式
            }
        }
        QString tempUrl = url.arg(ui->com_amapPrefix->currentText());                     // 设置域名
        tempUrl += QString("&style=%1").arg(style);                                       // 设置地图类型
        tempUrl += QString("&lang=%1").arg(ui->com_amapLang->currentData().toString());   // 设置语言
        tempUrl += QString("&scl=%1").arg(ui->com_amapScl->currentData().toInt());        // 设置图片尺寸，只在7 8生效
        tempUrl += QString("&ltype=%1").arg(ui->spin_amapLtype->value());                 // 设置图片中的信息，只有 7 8有效

        for (int x = ltX; x <= rdX; x++)
        {
            info.x = x;
            for (int y = ltY; y <= rdY; y++)
            {
                info.url = tempUrl + QString("&x=%1&y=%2&z=%3").arg(x).arg(y).arg(z);
                info.y = y;
                m_infos.append(info);
            }
        }
    }
}

/**
 * @brief 初始化Bing地图配置
 */
void MapInput::initBing()
{
    // 服务器
    for (int i = 0; i < 8; i++)
    {
        ui->com_bingPrefix->addItem(QString("%1").arg(i));
    }
    // 地图语言
    ui->com_bingLang->addItem("中文", "zh-cn");
    ui->com_bingLang->addItem("英语", "en-US");
    // 地图类型
    ui->com_bingType->addItem("卫星地图", "a");
    ui->com_bingType->addItem("普通地图", "r");
    ui->com_bingType->addItem("混合地图", "h");

    ui->com_bingCstl->addItem("默认", "w4c");
    ui->com_bingCstl->addItem("白天", "vb");    // 白天道路地图
    ui->com_bingCstl->addItem("夜晚", "vbd");   // 夜晚道路图
    // 瓦片等级
    for (int i = 1; i < 21; i++)
    {
        ui->com_bingZ->addItem(QString("%1").arg(i));
    }
    // 填入下载格式
    ui->com_bingFormat->addItem("jpg");
    ui->com_bingFormat->addItem("png");
    ui->com_bingFormat->addItem("bmp");
}

/**
 * @brief 计算Bing地图的下载信息（这些url可能会失效，后续会使用其他方式下载）
 *  https://learn.microsoft.com/en-us/bingmaps/rest-services/directly-accessing-the-bing-maps-tiles
 */
void MapInput::getBingMapInfo()
{
    //https://r1.tiles.ditu.live.com/tiles/r1321001.png?g=1001&mkt=zh-cn
    //http://dynamic.t2.tiles.ditu.live.com/comp/ch/r1321001.png?it=G,OS,L&mkt=en-us&cstl=w4c&ur=cn
    //http://ecn.t{0}.tiles.virtualearth.net/tiles/{1}{2}.png? g={4}
    //https://t0.dynamic.tiles.ditu.live.com/comp/ch/1320300313132?mkt=zh-CN&ur=CN&it=G,RL&n=z&og=894&cstl=vb
    //https://t1.dynamic.tiles.ditu.live.com/comp/ch/13203012200201?mkt=zh-CN&ur=cn&it=G,RL&n=z&og=894&cstl=vbd
    //https://dynamic.t1.tiles.ditu.live.com/comp/ch/1320300313313?it=Z,TF&L&n=z&key=AvquUWQgfy7VPqHn9ergJsp3Q_EiUft0ed70vZsX0_aqPABBdK07OkwrXWoGXsTG&ur=cn&cstl=vbd

#define USE_URL 1
#if (USE_URL == 0)
    // https://r1.tiles.ditu.live.com/tiles/r1321001.png?g=1001&mkt=zh-cn
    static QString url = "https://r%1.tiles.ditu.live.com/tiles/%2%3.%4?g=1001&mkt=%5";   // 街道图r支持中文
#elif (USE_URL == 1)
    // http://dynamic.t2.tiles.ditu.live.com/comp/ch/r1321001.png?it=G,OS,L&mkt=en-us&cstl=w4c&ur=cn
    static QString url = "http://dynamic.t%1.tiles.ditu.live.com/comp/ch/%2%3.%4?it=G,OS,L&mkt=%5&cstl=%6&ur=cn";
#endif
    int z = ui->com_bingZ->currentText().toInt();
    QStringList lt = ui->line_LTGps->text().trimmed().split(',');   // 左上角经纬度
    QStringList rd = ui->line_RDGps->text().trimmed().split(',');   // 右下角经纬度
    if (lt.count() != 2 || rd.count() != 2)
        return;                                    // 判断输入是否正确
    int ltX = lonTotile(lt.at(0).toDouble(), z);   // 计算左上角瓦片X
    int ltY = latTotile(lt.at(1).toDouble(), z);   // 计算左上角瓦片Y
    int rdX = lonTotile(rd.at(0).toDouble(), z);   // 计算右下角瓦片X
    int rdY = latTotile(rd.at(1).toDouble(), z);   // 计算右下角瓦片Y

    QString format = ui->com_bingFormat->currentText();
    ImageInfo info;
    info.z = z;
    info.format = format;
    int prefix = ui->com_bingPrefix->currentIndex();
    QString lang = ui->com_bingLang->currentData().toString();   // 语言
    QString type = ui->com_bingType->currentData().toString();   // 类型
    QString cstl = ui->com_bingCstl->currentData().toString();   // 样式

    QPoint point;
    for (int x = ltX; x <= rdX; x++)
    {
        info.x = x;
        point.setX(x);
        for (int y = ltY; y <= rdY; y++)
        {
            info.y = y;
            point.setY(y);
            QString quadKey = Bing::tileXYToQuadKey(point, z);   // 将xy转为quadkey
#if (USE_URL == 0)
            info.url = url.arg(prefix).arg(type).arg(quadKey).arg(format).arg(lang);
#elif (USE_URL == 1)
            info.url = url.arg(prefix).arg(type).arg(quadKey).arg(format).arg(lang).arg(cstl);
#endif
            m_infos.append(info);
        }
    }
}
