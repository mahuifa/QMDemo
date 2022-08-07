#include "controlbar.h"
#include "ui_controlbar.h"

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
