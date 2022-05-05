#include "playimage.h"

#include <QPainter>

PlayImage::PlayImage(QWidget *parent) : QWidget(parent)
{

}

/**
 * @brief        传入Qimage图片显示
 * @param image
 */
void PlayImage::updateImage(const QImage &image)
{
    m_pixmap = QPixmap::fromImage(image);
    update();
}

/**
 * @brief         传入QPixmap图片显示
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
    QPainter painter(this);
    painter.drawPixmap(this->rect(), m_pixmap);
    QWidget::paintEvent(event);
}
