#include "widget.h"
#include "ui_widget.h"
#include <QtConcurrent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");

    this->setWindowTitle(QString("Qt+OpenGL绘制三角形-QOpenGLWidget和QOpenGLWindow性能比较 V%1").arg(APP_VERSION));

    // 使用QOpenGLWidget绘制
    m_openglWidget = new OpenGLWidget;
    ui->gridLayout->addWidget(m_openglWidget);

    // 使用QOpenGLWindow绘制
    m_openglWindow = new OpenGLWindow;
    ui->gridLayout->addWidget(QWidget::createWindowContainer(m_openglWindow));

    connect(&m_timer, &QTimer::timeout, this, &Widget::on_timer);
    connect(this, &Widget::rotateWidget, m_openglWidget, &OpenGLWidget::rotate);
    connect(this, &Widget::rotateWindow, m_openglWindow, &OpenGLWindow::rotate);
}

Widget::~Widget()
{
    m_start = false;
    delete ui;
}

void  sleepMsec(int msec)
{
    QEventLoop loop;		//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();			//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void Widget::on_rotate()
{
    while(m_start)
    {
        if(m_isWindow)
        {
            emit rotateWindow();
        }
        if(m_isWidget)
        {
            emit rotateWidget();
        }
        sleepMsec(1);
    }
}

void Widget::on_timer()
{
    if(m_isWindow)
    {
        emit rotateWindow();
    }
    if(m_isWidget)
    {
        emit rotateWidget();
    }
}

void Widget::on_checkBox_window_stateChanged(int arg1)
{
    m_isWindow = arg1;
}

void Widget::on_checkBox_widget_stateChanged(int arg1)
{
    m_isWidget = arg1;
}

void Widget::on_but_thread_clicked()
{
    if(!m_start)
    {
        m_start = true;
        ui->but_thread->setText("停止");
        QtConcurrent::run(this, &Widget::on_rotate);
    }
    else
    {
        m_start =false;
        ui->but_thread->setText("线程刷新");
    }
}

void Widget::on_but_timer_clicked()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        ui->but_timer->setText("定时器刷新");
    }
    else
    {
        m_timer.start(1);
        ui->but_timer->setText("停止");
    }
}
