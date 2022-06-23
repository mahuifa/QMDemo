#include "test4.h"
#include "ui_test4.h"

Test4::Test4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Test4)
{
    ui->setupUi(this);
}

Test4::~Test4()
{
    delete ui;
}
