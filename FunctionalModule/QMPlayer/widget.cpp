#include "widget.h"
#include "ui_widget.h"
#include "QWindow"
#include <QDebug>
#include <QFile>
#include <QMouseEvent>
#include <QStyle>

Widget::Widget(QWidget *parent)
    : MWidgetBase(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();
    connectSlot();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    loadStyle();
    this->setWindowTitle(QString("QMPlayer V%1").arg(APP_VERSION));
    this->setTitleBar(ui->titleBar->getBackground());   // 设置标题栏
    ui->videoWidget->setMouseTracking(true);                       // 激活鼠标移动事件

    ui->videoWidget->installEventFilter(this);
}

void Widget::connectSlot()
{
    connect(this, &MWidgetBase::windowStateChanged, ui->titleBar, &TitleBar::on_windowStateChanged);
}

/**
 * @brief 加载样式表
 */
void Widget::loadStyle()
{
    QFile file(":/Style/main.css");
    if (file.open(QFile::ReadOnly))
    {
        //用QTextStream读取样式文件不用区分文件编码 带bom也行
        QStringList list;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line;
            in >> line;
            list << line;
        }

        file.close();
        QString qss = list.join("\n");
        this->setStyleSheet("");
        qApp->setStyleSheet(qss);
    }
}

/**
 * @brief       隐藏Widget设置标题文本功能，添加设置标题栏文本功能
 * @param title
 */
void Widget::setWindowTitle(const QString &title)
{
    QWidget::setWindowTitle(title);
    ui->titleBar->setWindowTitle(title);
}

/**
 * @brief  界面动态布局
 */
void Widget::windowLayout()
{
    // 设置组件大小
    int l_widget = ui->videoWidget->width() * 3 / 5;
    QSize size = ui->controlBar->size();
    size.setWidth(l_widget);
    ui->controlBar->resize(size);
    size.setHeight(ui->slider_video->height());
    ui->slider_video->resize(size);

    // 设置组件位置
    int x = (ui->videoWidget->width() - ui->controlBar->width()) / 2;
    int y = ui->videoWidget->height() - ui->controlBar->height() - 20;
    ui->controlBar->move(x, y);
    int sliderY = y - ui->slider_video->height() - 5;
    ui->slider_video->move(x, sliderY);

    // 侧边栏
    size = ui->sidebar->size();
    size.setHeight(sliderY - 20);
    ui->sidebar->resize(size);
    ui->sidebar->move(0, 0);
}

/**
 * @brief  全屏显示
 */
void Widget::showFullScreen()
{
    ui->titleBar->hide();
    MWidgetBase::showFullScreen();
    ui->slider_video->hide();
    ui->sidebar->hide();
    ui->controlBar->hide();
}

/**
 * @brief 全屏显示还原
 */
void Widget::showNormal()
{
    ui->titleBar->show();
    MWidgetBase::showNormal();
    ui->slider_video->show();
    ui->sidebar->show();
    ui->controlBar->show();          // 依赖于父窗口大小，必须放在全屏显示还原后面
}

/**
 * @brief         窗口显示事件
 * @param event
 */
void Widget::showEvent(QShowEvent *event)
{
    MWidgetBase::showEvent(event);
    windowLayout();
}

/**
 * @brief        窗口大小改变事件
 * @param event
 */
void Widget::resizeEvent(QResizeEvent *event)
{
    MWidgetBase::resizeEvent(event);
    windowLayout();
}

/**
 * @brief         事件过滤器
 * @param watched
 * @param event
 * @return
 */
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->videoWidget)
    {
        videoWidgetEvent(event);
    }

    return MWidgetBase::eventFilter(watched, event);
}

/**
 * @brief        处理窗口中的视频显示界面事件
 * @param event
 */
void Widget::videoWidgetEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonDblClick:
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        switch (e->button())
        {
        case Qt::LeftButton:           // 左键双击全屏显示、还原
        {
            if(this->isFullScreen())
            {
                this->showNormal();
            }
            else
            {
                this->showFullScreen();
            }
            break;
        }
        case Qt::RightButton:         // 右键双击显示隐藏控制栏、侧边栏、进度条
        {
            if(!ui->controlBar->isVisible())
            {
                ui->slider_video->show();
                ui->controlBar->show();
                ui->sidebar->show();
            }
            else
            {
                ui->slider_video->hide();
                ui->controlBar->hide();
                ui->sidebar->hide();
            }
            break;
        }
        default:break;
        }

        break;
    }
    case QEvent::MouseMove:
    {
        break;
    }
    default:break;
    }
}

