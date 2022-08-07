#include "controlbar.h"
#include "ui_controlbar.h"

#include <qdebug.h>
#include <qpropertyanimation.h>
#include <QStyle>

ControlBar::ControlBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlBar)
{
    ui->setupUi(this);

    m_paShow = new QPropertyAnimation(this, "pos");
    m_paShow->setDuration(500);
    connect(m_paShow, &QPropertyAnimation::finished, this, &ControlBar::on_finished);

}

ControlBar::~ControlBar()
{
    delete ui;
}

/**
 * @brief  打开显示动画（注意必须放在窗口改变后面，如全屏显示、还原）
 */
void ControlBar::show()
{
    QWidget::show();
    int y = this->parentWidget()->height() - this->height() - 20;
    m_paShow->setStartValue(QPoint(this->x(), this->parentWidget()->height()));
    m_paShow->setEndValue(QPoint(this->x(), y));
    m_paShow->setEasingCurve(QEasingCurve::OutQuad);
    m_paShow->start();
}

/**
 * @brief 关闭显示动画（注意必须放在窗口改变后面，如全屏显示、还原）
 */
void ControlBar::hide()
{
    int y = this->parentWidget()->height() - this->height() - 20;
    m_paShow->setStartValue(QPoint(this->x(), y));
    m_paShow->setEndValue(QPoint(this->x(), this->parentWidget()->height()));
    m_paShow->setEasingCurve(QEasingCurve::Linear);
    m_paShow->start();
}

void ControlBar::on_finished()
{
    if(m_paShow->endValue().toPoint().y() == this->parentWidget()->height())
    {
        QWidget::hide();
    }
}

void ControlBar::on_but_play_clicked()
{
    if(m_play)
    {

    }
    else
    {

    }

    m_play = !m_play;
    ui->but_play->setProperty("play", m_play);
    ui->but_play->style()->polish(ui->but_play);
}


void ControlBar::on_but_volume_clicked()
{
    m_volume = !m_volume;
    ui->but_volume->setProperty("setup", m_volume);
    ui->but_volume->style()->polish(ui->but_volume);
}

