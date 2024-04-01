#include "widget.h"
#include "ui_widget.h"
#include <qfiledialog.h>
#include <QApplication>
#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800, 600);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QT加载显示离线瓦片地图示例--V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 获取瓦片路径并显示
 */
void Widget::on_pushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "瓦片路径", qApp->applicationDirPath(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
        return;
    ui->lineEdit->setText(path);
    ui->graphicsView->setPath(path);
}
