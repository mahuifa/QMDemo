#include "playimage.h"

#include <QPainter>

PlayImage::PlayImage(QWidget *parent) : QWidget(parent)
{

}

/**
 * @brief        传入Qimage图片显示
 * @param image
 */
void PlayImage::updateImage(const QImage& image)
{
    updatePixmap(QPixmap::fromImage(image));
}

/**
 * @brief        传入QPixmap图片
 * @param pixmap
 */
void PlayImage::updatePixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

/**
 * @brief        使用Qpainter显示图片
 * @param event
 */
void PlayImage::paintEvent(QPaintEvent *event)
{
    if(!m_pixmap.isNull())
    {
        QPainter painter(this);
#if 0
        // 经过粗略测试，QImage先缩放后转为QPixmap的方式在图像比较小时耗时少，图片越大耗时远大
        QPixmap pixmap = QPixmap::fromImage(m_image.scaled(this->size(), Qt::KeepAspectRatio));
        // 先将QImage转换为QPixmap再进行缩放则耗时比较少，并且稳定，不会因为缩放图片大小而产生太大影响
        QPixmap pixmap1 = QPixmap::fromImage(m_image).scaled(this->size(), Qt::KeepAspectRatio);
#endif
        QPixmap pixmap = m_pixmap.scaled(this->size(), Qt::KeepAspectRatio);
        int x = (this->width() - pixmap.width()) / 2;
        int y = (this->height() - pixmap.height()) / 2;
        painter.drawPixmap(x, y, pixmap);
    }
    QWidget::paintEvent(event);
}
