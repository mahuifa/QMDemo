#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include <QDebug>
#include <QtMath>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1500, 1200);
    qApp->setStyleSheet("*{font: 12pt '宋体';}");
    this->setWindowTitle(QString("QT加载显示在线瓦片地图示例（绝对像素坐标）--V%1").arg(APP_VERSION));

    // 高德 地图源
    ui->com_url->addItem("https://wprd01.is.autonavi.com/appmaptile?&style=6&lang=zh_cn&scl=1&ltype=0&x={x}&y={y}&z={z}");
    ui->com_url->addItem("https://wprd01.is.autonavi.com/appmaptile?&style=7&lang=zh_cn&scl=1&ltype=0&x={x}&y={y}&z={z}");
    ui->com_url->addItem("https://wprd01.is.autonavi.com/appmaptile?&style=8&lang=zh_cn&scl=1&ltype=0&x={x}&y={y}&z={z}");
    ui->com_url->addItem("https://wprd01.is.autonavi.com/appmaptile?&style=9&lang=zh_cn&scl=1&ltype=0&x={x}&y={y}&z={z}");

    // Bing地图源
    ui->com_url->addItem("http://dynamic.t0.tiles.ditu.live.com/comp/ch/a{q}.jpg?it=G,OS,L&mkt=zh-cn&cstl=w4c&ur=cn");
    ui->com_url->addItem("http://dynamic.t0.tiles.ditu.live.com/comp/ch/r{q}.jpg?it=G,OS,L&mkt=zh-cn&cstl=w4c&ur=cn");
    ui->com_url->addItem("http://dynamic.t0.tiles.ditu.live.com/comp/ch/h{q}.jpg?it=G,OS,L&mkt=zh-cn&cstl=w4c&ur=cn");
    ui->com_url->addItem("http://dynamic.t0.tiles.ditu.live.com/comp/ch/h{q}.jpg?it=G,OS,L&mkt=zh-cn&cstl=vbd&ur=cn");
    ui->com_url->addItem("https://r0.tiles.ditu.live.com/tiles/a{q}.jpg?g=1001&mkt=zh-cn");   // Bing 卫星地图
    // ArcGis地图源
    ui->com_url->addItem("https://server.arcgisonline.com/arcgis/rest/services/NatGeo_World_Map/MapServer/tile/{z}/{y}/{x}.jpg");
    ui->com_url->addItem("https://server.arcgisonline.com/arcgis/rest/services/USA_Topo_Maps/MapServer/tile/{z}/{y}/{x}.jpg");
    ui->com_url->addItem("https://server.arcgisonline.com/arcgis/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}.jpg");
    ui->com_url->addItem("https://server.arcgisonline.com/arcgis/rest/services/World_Physical_Map/MapServer/tile/{z}/{y}/{x}.jpg");

    qRegisterMetaType<ImageInfo>("ImageInfo");
    m_geturl = new GetUrl();
    m_geturl->setUrl(ui->com_url->currentText());

    connect(GetUrlInterface::getInterface(), &GetUrlInterface::showRect, this, &Widget::showRect);
    connect(GetUrlInterface::getInterface(), &GetUrlInterface::setLevel, this,
            [&](int level)
            {
                int w = int(qPow(2, level) * 256);   // 最大范围
                ui->line_mapRange->setText(QString("[0，0]-[%1，%2]").arg(w).arg(w));
                ui->line_level->setText(QString("等级：%1").arg(level));
            });
}

Widget::~Widget()
{
    delete m_geturl;
    delete ui;
}

/**
 * @brief        下拉框设置地图源地址
 * @param index
 */
void Widget::on_com_url_activated(int index)
{
    Q_UNUSED(index);
    if (m_geturl)
    {
        m_geturl->setUrl(ui->com_url->currentText());
    }
}

/**
 * @brief       下拉框输入设置地图源地址
 * @param arg1
 */
void Widget::on_com_url_editTextChanged(const QString& arg1)
{
    if (m_geturl)
    {
        m_geturl->setUrl(arg1);
    }
}

/**
 * @brief        当前显示的地图像素范围
 * @param rect
 */
void Widget::showRect(QRect rect)
{
    QString str = QString("[%1，%2]-[%3，%4]").arg(rect.x()).arg(rect.y()).arg(rect.right()).arg(rect.bottom());
    ui->line_mapShowRect->setText(str);
}

/**
 * @brief 清空地图
 */
void Widget::on_but_clear_clicked()
{
    ui->graphicsView->clear();
}
