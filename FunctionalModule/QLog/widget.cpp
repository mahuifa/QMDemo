#include "widget.h"
#include "ui_widget.h"
#include <logsavetxt.h>
#include <qthread.h>
#include <QDebug>
#include <QtConcurrent>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("自定义日志系统 - V%1").arg(APP_VERSION));
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    //    LogSaveTxt::getInstance();   // 创建日志存储对象(无需窗口显示，只是保存)
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Widget::on_timer);
    m_timer->start(1000);
}

Widget::~Widget()
{
    m_quit = true;
    delete ui;
}

void Widget::on_timer()
{
    qDebug() << "debug信息";
    qInfo() << "Info信息";
    qWarning() << "Warning信息";
    qCritical() << "critical信息";
}

void Widget::on_com_FileType_activated(int index)
{
    ((LogSaveTxt*) LogSaveTxt::getInstance())->setFileType((LogSaveTxt::FileType) index);
}

void Widget::on_but_thread_clicked()
{
    QtConcurrent::run(QThreadPool::globalInstance(),
                      [&]()
                      {
                          for (int i = 0; i < INT_MAX; i++)
                          {
                              if (m_quit)
                                  return;
                              qInfo() << "adfjkla的撒法艰苦拉萨的飞机卡里的积分卡的距离" << i;
                              qInfo() << "adfjkla的撒法艰苦拉萨的飞机卡里的积分卡的距离" << i;
                              qInfo() << "adfjkla的撒法艰苦拉萨的飞机卡里的积分卡的距离" << i;
                              qInfo() << "adfjkla的撒法艰苦拉萨的飞机卡里的积分卡的距离" << i;
                              qInfo() << "adfjkla的撒法艰苦拉萨的飞机卡里的积分卡的距离" << i;
                              qInfo() << "adfjkla的撒法艰苦拉萨的飞机卡里的积分卡的距离" << i;
                              QThread::msleep(1);
                          }
                      });
}
