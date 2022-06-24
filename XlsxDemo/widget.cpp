#include "widget.h"
#include "ui_widget.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("QXlsx操作Excel 示例程序");
}

Widget::~Widget()
{
    delete ui;
}
