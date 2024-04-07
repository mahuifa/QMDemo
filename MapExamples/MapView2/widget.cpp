#include "widget.h"
#include "bingformula.h"
#include "ui_widget.h"

#include <qstring.h>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QWidgetAction>
#include <QtConcurrent>

static MapGraphicsView* g_view = nullptr;   // 当前视图

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1'200, 900);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QT加载显示离线瓦片地图示例（绝对像素坐标）--V%1").arg(APP_VERSION));

    g_view = ui->graphicsView;
    qRegisterMetaType<ImageInfo>("ImageInfo");

    connect(ui->graphicsView, &MapGraphicsView::zoom, this, &Widget::zoom);
    connect(ui->graphicsView, &MapGraphicsView::showRect, this, &Widget::showRect);
    connect(ui->graphicsView, &MapGraphicsView::mousePos, this, &Widget::mousePos);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_but_open_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "瓦片路径", qApp->applicationDirPath(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
        return;
    ui->line_path->setText(path);
    //    ui->graphicsView->setPath(path);

    m_levels = getDir(path);
    QString str;
    for (auto v : m_levels)
    {
        str += QString("%1,").arg(v);
    }
    ui->line_level->setText(str);
    if (!m_levels.isEmpty())   // 如果地图层级不为空，则加载第一级
    {
        m_index = 0;
        m_level = m_levels.first();
        getTitles(m_level);
        loadImages();
    }
}

/**
 * @brief         传入路径，获取文件夹下所有子文件夹名称，并排序放入数组
 * @param path
 * @return
 */
QVector<int> Widget::getDir(const QString& path)
{
    QVector<int> vector;
    QDir dir(path);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);   // 设置过滤类型为文件夹，且不包含隐藏文件夹
    QStringList dirs = dir.entryList();
    for (auto& strDir : dirs)
    {
        bool ok;
        int v = strDir.toInt(&ok);
        if (ok)
        {
            vector.append(v);
        }
    }
    qSort(vector.begin(), vector.end());
    return vector;
}

/**
 * @brief         传入路径，获取文件夹下所有文件名称，并排序放入数组
 * @param path
 * @return
 */
QVector<int> Widget::getFile(const QString& path)
{
    QVector<int> vector;
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);   // 设置过滤类型为文件，且不包含隐藏文件
    QStringList files = dir.entryList();
    for (auto& file : files)
    {
        QFileInfo info(file);
        // 将文件名转为数字
        bool ok;
        int v = info.baseName().toInt(&ok);
        if (ok)
        {
            vector.append(v);
        }
        // 获取所有存在的文件类型
        QString suffix = info.completeSuffix();
        if (ui->com_format->findText(suffix) < 0)
        {
            ui->com_format->addItem(suffix);
        }
    }
    qSort(vector.begin(), vector.end());
    return vector;
}

/**
 * @brief         获取当前层级的所有瓦片编号
 * @param level   地图层级Z
 */
void Widget::getTitles(int level)
{
    QString root = ui->line_path->text() + QString("/%1/").arg(level);
    QVector<int> titleX = getDir(root);   // 获取所有X层级

    m_tiles.clear();
    for (auto x : titleX)
    {
        QString path = root + QString("%1/").arg(x);
        QVector<int> titleY = getFile(path);
        for (auto y : titleY)
        {
            m_tiles.append(QPoint(x, y));
        }
    }
    QPoint lt = m_tiles.first();
    QPoint rd = m_tiles.last() + QPoint(1, 1);
    lt = Bing::tileXYToPixelXY(lt);
    rd = Bing::tileXYToPixelXY(rd);

    // 设置当前层级瓦片的范围
    QRect rect(lt.x(), lt.y(), rd.x() - lt.x(), rd.y() - lt.y());
    ui->graphicsView->setRect(rect);

    QPointF ltGPS = Bing::tileXYToLatLong(lt, level);
    QPointF rdGPS = Bing::tileXYToLatLong(rd, level);
    QString strRect = QString("[%1，%2] - [%3，%4] / ").arg(lt.x()).arg(lt.y()).arg(rd.x()).arg(rd.y());
    strRect += QString("[%1，%2] - [%3，%4]")
                   .arg(QString::number(ltGPS.x(), 'f', 6))
                   .arg(QString::number(ltGPS.y(), 'f', 6))
                   .arg(QString::number(rdGPS.x(), 'f', 6))
                   .arg(QString::number(rdGPS.y(), 'f', 6));
    ui->line_mapRange->setText(strRect);
}

/**
 * @brief         调用线程池加载瓦片
 * @param
 */
void getImage(ImageInfo& info)
{
    if (info.img.load(info.url))
    {
        emit g_view->updateImage(info);
    }
}

/**
 * @brief 加载所有瓦片图
 */
void Widget::loadImages()
{
    QString root = ui->line_path->text() + QString("/%1/").arg(m_level);
    QString format = ui->com_format->currentText();

    m_imageInfos.clear();
    ImageInfo info;
    info.z = m_level;
    for (auto& tile : m_tiles)
    {
        QString path = root + QString("%1/%2.%3").arg(tile.x()).arg(tile.y()).arg(format);
        info.url = path;
        info.x = tile.x();
        info.y = tile.y();
        m_imageInfos.append(info);
    }

    m_future = QtConcurrent::map(m_imageInfos, getImage);
}

void Widget::zoom(bool flag)
{
    // 判断之前的线程是否执行完成
    if (m_future.isRunning())
        return;
    // 判断缩放
    if (flag)
    {
        m_index++;
        if (m_index >= m_levels.count())
        {
            m_index = m_levels.count() - 1;
            return;
        }
    }
    else
    {
        m_index--;
        if (m_index < 0)
        {
            m_index = 0;
            return;
        }
    }

    ui->graphicsView->clear();        // 清除所有图元
    m_level = m_levels.at(m_index);   // 获取新的层级
    getTitles(m_level);               // 获取当前层级所有图元编号
    loadImages();                     // 在线程池中加载瓦片
}

/**
 * @brief        显示当前视图中的地图显示像素范围（场景坐标）
 * @param rect
 */
void Widget::showRect(QRect rect)
{
    QString str = QString("[%1，%2] - [%3，%4]").arg(rect.x()).arg(rect.y()).arg(rect.left()).arg(rect.bottom());
    ui->line_mapShowRect->setText(str);
}

/**
 * @brief     显示当前鼠标位置（场景坐标）
 * @param pos
 */
void Widget::mousePos(QPoint pos)
{
    ui->line_mousePos->setText(QString("[%1，%2]").arg(pos.x()).arg(pos.y()));
}
