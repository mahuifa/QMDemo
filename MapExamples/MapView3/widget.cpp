#include "widget.h"
#include "ui_widget.h"
#include <QApplication>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1'200, 900);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QT加载显示离线瓦片地图示例（绝对像素坐标）--V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}
