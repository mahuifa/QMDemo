#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    addOp(QPainter::CompositionMode_SourceOver,       "SourceOver");
    addOp(QPainter::CompositionMode_DestinationOver,  "DestinationOver");
    addOp(QPainter::CompositionMode_Clear,            "Clear");
    addOp(QPainter::CompositionMode_Source,           "Source");
    addOp(QPainter::CompositionMode_Destination,      "Destination");
    addOp(QPainter::CompositionMode_SourceIn,         "SourceIn");
    addOp(QPainter::CompositionMode_DestinationIn,    "DestinationIn");
    addOp(QPainter::CompositionMode_SourceOut,        "SourceOut");
    addOp(QPainter::CompositionMode_DestinationOut,   "DestinationOut");
    addOp(QPainter::CompositionMode_SourceAtop,       "SourceAtop");
    addOp(QPainter::CompositionMode_DestinationAtop,  "DestinationAtop");
    addOp(QPainter::CompositionMode_Xor,              "Xor");

    // svg 1.2混合模式
    addOp(QPainter::CompositionMode_Plus,             "Plus");
    addOp(QPainter::CompositionMode_Multiply,         "Multiply");
    addOp(QPainter::CompositionMode_Screen,           "Screen");
    addOp(QPainter::CompositionMode_Overlay,          "Overlay");
    addOp(QPainter::CompositionMode_Darken,           "Darken");
    addOp(QPainter::CompositionMode_Lighten,          "Lighten");
    addOp(QPainter::CompositionMode_ColorDodge,       "ColorDodge");
    addOp(QPainter::CompositionMode_ColorBurn,        "ColorBurn");
    addOp(QPainter::CompositionMode_HardLight,        "HardLight");
    addOp(QPainter::CompositionMode_SoftLight,        "SoftLight");
    addOp(QPainter::CompositionMode_Difference,       "Difference");
    addOp(QPainter::CompositionMode_Exclusion,        "Exclusion");

    // ROPs
    addOp(QPainter::RasterOp_SourceOrDestination,         "SourceOrDestination");
    addOp(QPainter::RasterOp_SourceAndDestination,        "SourceAndDestination");
    addOp(QPainter::RasterOp_SourceXorDestination,        "SourceXorDestination");
    addOp(QPainter::RasterOp_NotSourceAndNotDestination,  "NotSourceAndNotDestination");
    addOp(QPainter::RasterOp_NotSourceOrNotDestination,   "NotSourceOrNotDestination");
    addOp(QPainter::RasterOp_NotSourceXorDestination,     "NotSourceXorDestination");
    addOp(QPainter::RasterOp_NotSource,                   "NotSource");
    addOp(QPainter::RasterOp_NotSourceAndDestination,     "NotSourceAndDestination");
    addOp(QPainter::RasterOp_SourceAndNotDestination,     "SourceAndNotDestination");
    addOp(QPainter::RasterOp_NotSourceOrDestination,      "NotSourceOrDestination");
    addOp(QPainter::RasterOp_SourceOrNotDestination,      "SourceOrNotDestination");
    addOp(QPainter::RasterOp_ClearDestination,            "ClearDestination");
    addOp(QPainter::RasterOp_SetDestination,              "SetDestination");
    addOp(QPainter::RasterOp_NotDestination,              "NotDestination");

}

Widget::~Widget()
{
    delete ui;
}

void Widget::addOp(QPainter::CompositionMode mode, const QString &name)
{
    ui->operatorComboBox->addItem(name, mode);
}

void Widget::loadImage(const QString &fileName, QImage &image, QToolButton *button)
{
    if(!image.load(fileName))   // 加载图片
    {
        return;
    }
    image = image.scaled(ui->sourceButton->size(), Qt::KeepAspectRatio);

}


void Widget::on_sourceButton_clicked()
{

}


void Widget::on_destinationButton_clicked()
{

}

void Widget::on_operatorComboBox_currentIndexChanged(int index)
{

}
