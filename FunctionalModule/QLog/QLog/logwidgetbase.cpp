#include "logwidgetbase.h"
#include "loginput.h"

LogWidgetBase::LogWidgetBase(QWidget *parent) : QWidget(parent)
{
    LogSaveTxt::initLog();   // 初始化日志保存功能
    connect(LogInput::getInstance(), &LogInput::logData, this, &LogWidgetBase::on_logData, Qt::QueuedConnection);
}
