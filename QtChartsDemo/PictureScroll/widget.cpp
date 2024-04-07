#include "widget.h"
#include "ui_widget.h"
#include <QRandomGenerator>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("QtCharts绘图-画卷式绘图Demo - V%1").arg(APP_VERSION));

    connect(&m_timer, &QTimer::timeout, this, &Widget::on_timeout);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    m_timer.start(1);
}

void Widget::on_timeout()
{
    static int i = -100;
    QRandomGenerator* random = QRandomGenerator::global();
    QPointF pointf(random->bounded(0, 100), random->bounded(i, i + 20));
    ChartView::DataType type =  ChartView::DataType(random->bounded(0, 3));
    if(i % 2 == 0)
    {
        ui->graphicsView->addData(pointf, type, ":/1.jpg");     // 添加数据点
    }
    else
    {
        ui->graphicsView->addData(pointf, type, ":/1.png");     // 添加数据点
    }

    i++;
    if(i == 200)
    {
        m_timer.stop();
        i = 0;
    }
}
