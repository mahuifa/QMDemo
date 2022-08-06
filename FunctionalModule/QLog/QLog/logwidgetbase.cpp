#include "logwidgetbase.h"
#include "loginput.h"

LogWidgetBase::LogWidgetBase(QWidget *parent) : QWidget(parent)
{
    LogSaveTxt::getInstance();   // 创建日志存储对象
    connect(LogInput::getInstance(), &LogInput::logData, this, &LogWidgetBase::on_logData, Qt::QueuedConnection);
}
