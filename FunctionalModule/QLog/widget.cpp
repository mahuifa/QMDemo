#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <logsavetxt.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("自定义日志系统 - V%1").arg(APP_VERSION));
//    LogSaveTxt::getInstance();   // 创建日志存储对象(无需窗口显示，只是保存)
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Widget::on_timer);
    m_timer->start(1000);
}

Widget::~Widget()
{
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
    ((LogSaveTxt*)LogSaveTxt::getInstance())->setFileType((LogSaveTxt::FileType)index);
}

