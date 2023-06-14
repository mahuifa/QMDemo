#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("Qt使用QPainter绘制图片Demo - V%1").arg(APP_VERSION));

    m_pixmap[0] = QPixmap::fromImage(QImage(":/0.jpg"));
    m_pixmap[1] = QPixmap::fromImage(QImage(":/1.jpg"));
    connect(&m_timer, &QTimer::timeout, this, &Widget::on_timerout);
    m_timer.start(20);
    m_playImages = this->findChildren<PlayImage *>();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_timerout()
{
    static bool flag = true;
    for(int i = 0; i < m_playImages.count(); i++)
    {
        if(flag)
        {
            m_playImages.at(i)->updatePixmap(m_pixmap[0]);
        }
        else
        {
            m_playImages.at(i)->updatePixmap(m_pixmap[1]);
        }
    }
    flag = !flag;
}

