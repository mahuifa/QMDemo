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


}

ControlBar::~ControlBar()
{
    delete ui;
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

