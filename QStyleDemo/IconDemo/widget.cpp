#include "widget.h"
#include "ui_widget.h"
#include <QStyle>
#include <QDebug>
#include <QMetaEnum>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->resize(400, 300);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("Qt内置图标演示Demo - V%1").arg(APP_VERSION));
    // 将枚举的名称和数值添加进下拉列表中
    QMetaEnum m = QMetaEnum::fromType<QStyle::StandardPixmap>();
    for(int i=0; i < m.keyCount(); ++i)
    {
        QStyle::StandardPixmap icon = QStyle::StandardPixmap(i);   // 下拉框选择的枚举
        ui->com_iconEnum->addItem(this->style()->standardIcon(icon), m.key(i), m.value(i));  // 在下拉框中添加内置图标、对应的枚举名称、值
    }
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief        通过下拉框切换图标
 * @param index
 */
void Widget::on_com_iconEnum_activated(int index)
{
    QStyle::StandardPixmap icon = QStyle::StandardPixmap(ui->com_iconEnum->itemData(index).toInt());   // 下拉框选择的枚举
    ui->pushButton->setIcon(this->style()->standardIcon(icon));
}
