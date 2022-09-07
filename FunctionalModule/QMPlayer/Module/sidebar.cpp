#include "sidebar.h"
#include "ui_sidebar.h"
#include <QPropertyAnimation>
#include <qdebug.h>

Sidebar::Sidebar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sidebar)
{
    ui->setupUi(this);
}

Sidebar::~Sidebar()
{
    delete ui;
}

