#include "slidervideo.h"

#include <QMouseEvent>
#include <QStyle>

SliderVideo::SliderVideo(QWidget *parent) : QSlider(parent)
{
}


void SliderVideo::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton || event->button() == Qt::MidButton)       // 右键或中键点击时跳转到鼠标点击位置
    {
        int value = QStyle::sliderValueFromPosition(this->minimum(), this->maximum(), event->pos().x(), this->width());
        this->setValue(value);
    }
    QSlider::mousePressEvent(event);
}
