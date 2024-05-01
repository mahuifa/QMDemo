#include "widget.h"
#include "qrencode.h"   // 添加头文件
#include "ui_widget.h"
#include <qfiledialog.h>
#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("QT生成、显示二维码示例");
    this->setWindowTitle(QString("QT生成、显示二维码自定义控件示例 - V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->qrCodeView->setString(ui->lineEdit->text());   // 生成二维码
}

void Widget::on_but_save_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "二维码保存到", "./二维码.jpg", "图片 (*.jpg)");
    if (path.isEmpty())
        return;

    ui->qrCodeView->save(path);   // 保存二维码
}
