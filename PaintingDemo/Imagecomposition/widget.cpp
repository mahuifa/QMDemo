#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>

static const QSize resultSize(200, 200);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QPainter图像合成Demo - V%1").arg(APP_VERSION));

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

    ui->sourceButton->setFixedSize(resultSize);
    ui->destinationButton->setFixedSize(resultSize);
    ui->sourceButton->setIconSize(resultSize);
    ui->destinationButton->setIconSize(resultSize);

    resultImage = QImage(resultSize, QImage::Format_ARGB32_Premultiplied);       // 初始化用于绘制合成结果的图片对象

    loadImage(":/images/butterfly.png", sourceImage, ui->sourceButton);
    loadImage(":/images/checker.png", destinationImage, ui->destinationButton);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::addOp(QPainter::CompositionMode mode, const QString &name)
{
    ui->operatorComboBox->addItem(name, mode);
}

/**
 * @brief             加载图片
 * @param fileName    图片路径
 * @param image       保存图片的对象
 * @param button      显示图片的控件
 */
void Widget::loadImage(const QString &fileName, QImage &image, QToolButton *button)
{
    if(!image.load(fileName))   // 加载图片
    {
        return;
    }
    image = image.scaled(ui->sourceButton->size(), Qt::KeepAspectRatio);    // 缩放图片

    QImage fixedImage(ui->sourceButton->size(), QImage::Format_ARGB32_Premultiplied);  // 使用预乘32位ARGB格式（0xAARGGBB）存储图像,某些操作（例如使用alpha混合的图像合成）使用预乘ARGB32比使用普通ARGB32更快。
    QPainter painter(&fixedImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);           // 设置图片合成模式，只有QImage支持
    painter.fillRect(fixedImage.rect(), Qt::transparent);                   // 填充透明
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(imagePos(image), image);                              // 画图片

    image = fixedImage;

    button->setIcon(QPixmap::fromImage(image));                             // 在but上显示加载的图片

    recalculateResult();
}

QPointF Widget::imagePos(const QImage &image) const
{
    return QPointF(qreal(ui->sourceButton->width() - image.width()) / 2, qreal(ui->sourceButton->height() - image.height()) / 2);
}

/**
 * @brief  图片合成
 */
void Widget::recalculateResult()
{
    QPainter painter(&resultImage);
    // 输出是源像素。
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);

    // 这是默认模式。 源的 alpha 用于混合目标顶部的像素
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, destinationImage);

    painter.setCompositionMode(QPainter::CompositionMode(ui->operatorComboBox->currentData().toInt()));      // 通过下拉框设置合成模式
    painter.drawImage(0, 0, sourceImage);

    // 目标的 alpha 用于将其混合到源像素的顶部。 此模式与 CompositionMode_SourceOver 相反。
    painter.setCompositionMode(QPainter::CompositionMode_Destination);
    painter.fillRect(resultImage.rect(), Qt::white);

    ui->resultLabel->setPixmap(QPixmap::fromImage(resultImage));
}


void Widget::on_sourceButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择源图像");
    if (!fileName.isEmpty())
    {
        loadImage(fileName, sourceImage, ui->sourceButton);
    }
}


void Widget::on_destinationButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择目标图像");
    if (!fileName.isEmpty())
    {
        loadImage(fileName, destinationImage, ui->destinationButton);
    }
}

/**
 * @brief       切换合成模式
 * @param index
 */
void Widget::on_operatorComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    recalculateResult();
}
