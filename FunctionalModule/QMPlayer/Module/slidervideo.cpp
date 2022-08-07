#include "slidervideo.h"

#include <QMouseEvent>
#include <QStyle>
#include <qpropertyanimation.h>

SliderVideo::SliderVideo(QWidget *parent) : QSlider(parent)
{
    m_paShow = new QPropertyAnimation(this, "size");
    m_paShow->setDuration(500);
    connect(m_paShow, &QPropertyAnimation::finished, this, &SliderVideo::on_finished);
}

/**
 * @brief  打开显示动画（注意必须放在窗口改变后面，如全屏显示、还原）
 */
void SliderVideo::show()
{
    QWidget::show();
    m_paShow->setStartValue(QSize(0, this->height()));
    QSize size = this->size();
    size.setWidth(this->parentWidget()->width() * 3 / 5);
    m_paShow->setEndValue(size);
    m_paShow->setEasingCurve(QEasingCurve::OutQuad);
    m_paShow->start();
}

/**
 * @brief 关闭显示动画（注意必须放在窗口改变后面，如全屏显示、还原）
 */
void SliderVideo::hide()
{
    QSize size = this->size();
    size.setWidth(this->parentWidget()->width() * 3 / 5);
    m_paShow->setStartValue(size);
    m_paShow->setEndValue(QSize(0, this->height()));
    m_paShow->setEasingCurve(QEasingCurve::OutQuad);
    m_paShow->start();
}

void SliderVideo::on_finished()
{
    if(m_paShow->endValue().toSize().width() == 0)
    {
        QWidget::hide();
    }
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
