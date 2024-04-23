#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1200, 900);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QT加载显示离线瓦片地图示例（绝对像素坐标）--V%1").arg(APP_VERSION));

    ui->com_url->addItem("https://server.arcgisonline.com/arcgis/rest/services/NatGeo_World_Map/MapServer/tile/{z}/{y}/{x}.jpg");   // ArcGis
    ui->com_url->addItem("https://wprd01.is.autonavi.com/appmaptile?&style=6&lang=zh_cn&scl=1&ltype=0&x={x}&y={y}&z={z}");          // 高德
    ui->com_url->addItem("http://dynamic.t0.tiles.ditu.live.com/comp/ch/a{q}.jpg?it=G,OS,L&mkt=zh-cn&cstl=w4c&ur=cn");              // Bing

    qRegisterMetaType<ImageInfo>("ImageInfo");
    m_geturl = new GetUrl();
    m_geturl->setUrl(ui->com_url->currentText());
}

Widget::~Widget()
{
    delete m_geturl;
    delete ui;
}

/**
 * @brief        设置地图源地址
 * @param index
 */
void Widget::on_com_url_activated(int index)
{
    Q_UNUSED(index);
    m_geturl->setUrl(ui->com_url->currentText());
}
