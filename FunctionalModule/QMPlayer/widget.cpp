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
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    loadStyle();
    this->setWindowTitle(QString("QMPlayer - V%1").arg(APP_VERSION));
    this->setTitleBar(ui->titleBar->getBackground());              // 设置标题栏
    ui->videoWidget->setMouseTracking(true);                       // 激活鼠标移动事件

    ui->videoWidget->installEventFilter(this);

    m_paSlider = new QPropertyAnimation(ui->slider_video, "size");
    m_paControlBar = new QPropertyAnimation(ui->controlBar, "size");
    m_paSidebar = new QPropertyAnimation(ui->sidebar, "pos");
    m_paSlider->setDuration(500);
    m_paControlBar->setDuration(500);
    m_paSidebar->setDuration(500);
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
    ui->controlBar->resize(size);                        // 设置控制栏大小
    size.setHeight(ui->slider_video->height());
    ui->slider_video->resize(size);                      // 设置进度条大小

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
    m_visible = true;
}

void Widget::dynamicShowNormal()
{
    if(ui->controlBar->width() > 0)
    {
        dynamicHide();
    }
    else
    {
        dynamicShow();
    }
}


void Widget::dynamicHide()
{
    if(!m_visible)
    {
        return;
    }

    // 隐藏进度条
    m_paSlider->setStartValue(ui->slider_video->size());
    m_paSlider->setEndValue(QSize(0, ui->slider_video->height()));
    m_paSlider->setEasingCurve(QEasingCurve::OutQuad);
    m_paSlider->start();

    // 隐藏控制栏
    m_paControlBar->setStartValue(ui->controlBar->size());
    m_paControlBar->setEndValue(QSize(0, ui->controlBar->height()));
    m_paControlBar->setEasingCurve(QEasingCurve::OutQuad);
    m_paControlBar->start();

    // 隐藏侧边栏
    m_paSidebar->setStartValue(QPoint(0, 0));
    m_paSidebar->setEndValue(QPoint(-ui->sidebar->width(), 0));
    m_paSidebar->setEasingCurve(QEasingCurve::OutQuad);
    m_paSidebar->start();

    m_visible = false;
}

void Widget::dynamicShow()
{
    if(m_visible)
    {
        return;
    }

    int l_widget = ui->videoWidget->width() * 3 / 5;
    m_paSlider->setStartValue(QSize(0, ui->slider_video->height()));
    m_paSlider->setEndValue(QSize(l_widget, ui->slider_video->height()));
    m_paSlider->setEasingCurve(QEasingCurve::OutQuad);
    m_paSlider->start();

    m_paControlBar->setStartValue(QSize(0, ui->controlBar->height()));
    m_paControlBar->setEndValue(QSize(l_widget, ui->controlBar->height()));
    m_paControlBar->setEasingCurve(QEasingCurve::OutQuad);
    m_paControlBar->start();

    m_paSidebar->setStartValue(QPoint(-ui->sidebar->width(), 0));
    m_paSidebar->setEndValue(QPoint(0, 0));
    m_paSidebar->setEasingCurve(QEasingCurve::OutQuad);
    m_paSidebar->start();

    m_visible = true;
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
                ui->titleBar->show();
                this->showNormal();
                dynamicShow();
            }
            else
            {
                ui->titleBar->hide();
                this->showFullScreen();
                dynamicHide();
            }
            break;
        }
        case Qt::RightButton:         // 右键双击显示隐藏控制栏、侧边栏、进度条
        {
            dynamicShowNormal();
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

