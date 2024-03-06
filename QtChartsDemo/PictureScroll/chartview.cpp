#include "chartview.h"

#include <QValueAxis>
#include <QGraphicsScene>
#include <QDebug>
#include <qpushbutton.h>
#include <QGraphicsProxyWidget>
#include <QDir>
#include <QDateTime>
#include <QBuffer>
#include <QFileDialog>
#include <qscrollbar.h>
#include <QtConcurrent>
#include <QMessageBox>

const int g_margins = 20;         // 数据点到Y轴上下边距
const int g_zoom = 10;            // Y轴缩放步进
const int g_range = 200;          // Y轴显示范围

ChartView::ChartView(QWidget *parent) : QChartView(parent)
{
    m_series1 = new QScatterSeries();
    m_series2 = new QScatterSeries();
    m_series3 = new QScatterSeries();

    m_series1->setBrush(QBrush(Qt::red));
    m_series2->setBrush(QBrush(Qt::cyan));
    m_series3->setBrush(QBrush(Qt::green));

    m_series1->setName("样式1");
    m_series2->setName("样式2");
    m_series3->setName("样式3");

    this->chart()->addSeries(m_series1);
    this->chart()->addSeries(m_series2);
    this->chart()->addSeries(m_series3);

    //定义坐标轴
     m_xAxis = new QValueAxis();
     m_yAxis = new QValueAxis();

     m_maxY = m_minY + g_range;
     //设置坐标轴间隔
     m_xAxis->setTickCount(11);                 // 刻度个数，
     m_xAxis->setMinorTickCount(9);             // 子刻度
     m_xAxis->setRange(0, 100);                 // 坐标轴范围
     m_xAxis->setLabelFormat("%d");             //设置刻度的格式，整数形式
     m_xAxis->setMinorGridLineVisible(false);   // 不显示子网格线

     m_yAxis->setTickCount(11);
     m_yAxis->setMinorTickCount(9);
     m_yAxis->setMinorGridLineVisible(false);
     m_yAxis->setRange(m_minY, m_maxY);
     m_yAxis->setLabelFormat("%d");
     this->chart()->addAxis(m_xAxis, Qt::AlignBottom);
     this->chart()->addAxis(m_yAxis, Qt::AlignRight);

     // 将坐标轴附着到序列
     m_series1->attachAxis(m_xAxis);
     m_series1->attachAxis(m_yAxis);
     m_series2->attachAxis(m_xAxis);
     m_series2->attachAxis(m_yAxis);
     m_series3->attachAxis(m_xAxis);
     m_series3->attachAxis(m_yAxis);

     connect(m_series1, &QScatterSeries::clicked, this, &ChartView::handleClickedPoint);
     connect(m_series2, &QScatterSeries::clicked, this, &ChartView::handleClickedPoint);
     connect(m_series3, &QScatterSeries::clicked, this, &ChartView::handleClickedPoint);

     QPushButton* but1 = new QPushButton("保存当前卷");
     QGraphicsProxyWidget* widget = this->chart()->scene()->addWidget(but1);
     widget->setPos(10, 10);

     QPushButton* but2 = new QPushButton("重置");
     widget = this->chart()->scene()->addWidget(but2);
     widget->setPos(90, 10);

     QPushButton* but3 = new QPushButton("加载数据");
     widget = this->chart()->scene()->addWidget(but3);
     widget->setPos(170, 10);

     m_maxReality = m_minReality + g_range;
     m_scrollBar = new QScrollBar();
     m_scrollBar->setRange(m_minReality, m_maxReality);
     m_scrollBar->setValue(m_maxReality);
     m_scrollBar->setPageStep(m_maxReality);
     m_scrollBar->setInvertedAppearance(true);
     widget = this->chart()->scene()->addWidget(m_scrollBar);
     widget->setPos(10, 80);

     connect(but1, &QPushButton::clicked, this, &ChartView::on_saveData);
     connect(but2, &QPushButton::clicked, this, &ChartView::on_reset);
     connect(but3, &QPushButton::clicked, this, &ChartView::on_loadData);
     connect(m_scrollBar, &QScrollBar::valueChanged, this, &ChartView::on_valueChanged);
     connect(this, &ChartView::showLog, this, &ChartView::on_showLog);
     connect(this, &ChartView::add, this, QOverload<const Data&>::of(&ChartView::addData));

     qRegisterMetaType<Data>("Data");
}

ChartView::~ChartView()
{
}

/**
 * @brief       添加数据
 * @param data
 */
void ChartView::addData(const Data &data)
{
    m_datas.append(data);

    switch (data.type)
    {
    case Type1:
    {
        m_series1->append(data.pointf);
        break;
    }
    case Type2:
    {
        m_series2->append(data.pointf);
        break;
    }
    case Type3:
    {
        m_series3->append(data.pointf);
        break;
    }
    }

    // 动态调整y轴范围
    if(data.pointf.y() > (m_maxY - g_margins))
    {
        m_maxReality = static_cast<int>(data.pointf.y() + g_margins);
    }
    if(data.pointf.y() < (m_minReality + g_margins))
    {
        m_minReality = static_cast<int>(data.pointf.y() - g_margins);
    }
    m_scrollBar->setRange(m_minReality, m_maxReality);
    m_scrollBar->setValue(m_maxReality);
}

void ChartView::addData(const QPointF &point, ChartView::DataType type, const QString &imgPath)
{
    // 创建一个数据
    Data data;
    data.pointf = point;
    data.pixmap = QPixmap(imgPath);
    data.type =  type;
    addData(data);
}


/**
 * @brief         鼠标点击绘制的点，显示对应的图片
 * @param point
 */
void ChartView::handleClickedPoint(const QPointF &point)
{
    for(int i = 0; i < m_datas.count(); i++)
    {
        if(m_datas.at(i).pointf == point)  // 在列表中查找与点击点相同的数据
        {
            if(m_datas.at(i).item)      // 如果图元已经存在，则切换显示状态
            {
                m_datas.at(i).item->setVisible(!m_datas.at(i).item->isVisible());
                m_datas.at(i).item->setPos(this->chart()->mapToScene(this->mapFromGlobal(QCursor::pos())) + QPoint(2, 2));
                return;
            }
            if(m_datas.at(i).pixmap.isNull())     // 如果没有图片则不显示
            {
                return;
            }
            // 如果图元不存在，则添加显示图片的图元
            QGraphicsScene *scene = this->chart()->scene();
            m_datas[i].item = scene->addPixmap(m_datas.at(i).pixmap);
            m_datas.at(i).item->setPos(this->chart()->mapToScene(this->mapFromGlobal(QCursor::pos())) + QPoint(2, 2));
        }
    }
}


/**
 * @brief        使用鼠标滚轮缩放Y轴范围
 * @param event
 */
void ChartView::wheelEvent(QWheelEvent *event)
{
    QChartView::wheelEvent(event);

    if(event->angleDelta().y() > 0)
    {
        m_yAxis->setRange(m_yAxis->min() - g_zoom, m_yAxis->max() + g_zoom);
    }
    else
    {
        m_yAxis->setRange(m_yAxis->min() + g_zoom, m_yAxis->max() - g_zoom);
    }

    hidePixmap();
}

/**
 * @brief          窗口大小改变时自动修改滑动条大小
 * @param event
 */
void ChartView::resizeEvent(QResizeEvent *event)
{
    QChartView::resizeEvent(event);

    m_scrollBar->resize(15, static_cast<int>(this->chart()->scene()->height() - 100));
}

/**
 * @brief 将数据保存到文件中
 */
void ChartView::on_saveData()
{
    if(m_datas.count() == 0) return;

    QString path = QDir("./Data").absolutePath();
    QDir dir;
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }

    // 在线程中保存数据
    QtConcurrent::run(QThreadPool::globalInstance(), [=]()
    {
        QString name = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") + ".csv";
        QFile file(path + "/" + name);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            for(auto data : m_datas)  // 如果没有保存完就退出程序会导致程序崩溃
            {
                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QIODevice::WriteOnly);
                data.pixmap.save(&buffer, "PNG"); // 你可以选择其他格式，如JPEG
                QString str = QString("%1,%2,%3,%4\n").arg(data.pointf.x()).arg(data.pointf.y()).arg(data.type)
                                                    .arg(byteArray.toBase64().data());

                file.write(str.toStdString().data(), str.size());
            }
            file.close();
            emit showLog("保存数据完成");
        }
        else
        {
            qDebug() << "文件打开失败";
        }
    });

}

/**
 * @brief 从文件中加载数据并显示
 */
void ChartView::on_loadData()
{
    QString path = QDir("./Data").absolutePath();
    QString strName = QFileDialog::getOpenFileName(this, "选择数据文件", path, "数据文件 (*.csv)");
    if(strName.isEmpty()) return;
    on_reset();

    // 在线程中 加载数据
    QtConcurrent::run(QThreadPool::globalInstance(), [=]()
    {
        QFile file(strName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while (!file.atEnd())
            {
                QString str = file.readLine();
                QStringList list = str.split(",");
                Data data;
                data.pointf.setX(list.at(0).toInt());
                data.pointf.setY(list.at(1).toInt());
                data.type = (DataType)list.at(2).toInt();
                QByteArray arr = QByteArray(list.at(3).toStdString().data(), list.at(3).count());
                data.pixmap.loadFromData(QByteArray::fromBase64(arr));
                emit add(data);
            }
            file.close();
            emit showLog("加载数据完成");
        }
        else
        {
            qDebug() << "打开文件失败";
        }
    });
}

/**
 * @brief 重置绘制界面
 */
void ChartView::on_reset()
{
    m_datas.clear();
    m_series1->clear();
    m_series2->clear();
    m_series3->clear();

    m_minY = 0;
    m_maxY = m_minY + g_range;
    m_minReality = 0;
    m_maxReality = m_minReality + g_range;
    m_scrollBar->setRange(m_minReality, m_maxReality);
    m_scrollBar->setValue(m_maxReality);
    m_yAxis->setRange(0, 100);

    // 清空显示图片的图元
    QList<QGraphicsItem *> items = this->chart()->scene()->items();
    for(int i = 0; i < items.count(); i++)
    {
        if(items.at(i)->type() == QGraphicsPixmapItem::Type)
        {
            this->chart()->scene()->removeItem(items[i]);
            delete items[i];
        }
    }
}

/**
 * @brief 关闭显示图片的图元
 */
void ChartView::hidePixmap()
{
    QList<QGraphicsItem *> items = this->chart()->scene()->items();
    for(int i = 0; i < items.count(); i++)
    {
        if(items.at(i)->type() == QGraphicsPixmapItem::Type)
        {
            items.at(i)->setVisible(false);
        }
    }
}

/**
 * @brief         滑动条控制显示范围
 * @param value
 */
void ChartView::on_valueChanged(int value)
{
    m_maxY = value;
    m_minY = value - g_range;
    m_yAxis->setRange(m_minY, m_maxY);
    hidePixmap();
}

void ChartView::on_showLog(QString str)
{
    QMessageBox::about(nullptr, "提示", str);
}
