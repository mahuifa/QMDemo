#include "widget.h"
#include "ui_widget.h"

#include <QDateTime>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(600, 400);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QT下载瓦片地图简单示例--V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 获取下载路径
 */
void Widget::on_but_savePath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "下载路径",
                                               qApp->applicationDirPath(),
                                               QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(path.isEmpty()) return;
    ui->line_savePath->setText(path + "/map/");
}

/**
 * @brief          单线程下载瓦片地图
 * @param checked
 */
void Widget::on_but_thread_clicked(bool checked)
{
    if(checked)
    {
        ui->but_thread->setText("停止下载");
    }
    else
    {
        ui->but_thread->setText("单线程下载");
        ui->but_thread->setChecked(false);
    }
}

/**
 * @brief          多线程下载瓦片地图
 * @param checked
 */
void Widget::on_but_threads_clicked(bool checked)
{
    if(checked)
    {
        ui->but_threads->setText("停止下载");
    }
    else
    {
        ui->but_threads->setText("多线程下载");
        ui->but_threads->setChecked(false);
    }
}
