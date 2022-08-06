#include "logsavebase.h"

#include <QDir>
#include <qmutex.h>
#include "loginput.h"

LogSaveBase::LogSaveBase(QObject *parent) : QObject(parent)
{
    QDir dir;
    if(!dir.exists(LOG_PATH))
    {
        dir.mkpath(LOG_PATH);
    }
    connect(LogInput::getInstance(), &LogInput::logData, this, &LogSaveBase::on_logData, Qt::QueuedConnection);
}

LogSaveBase::~LogSaveBase()
{
    disconnect(LogInput::getInstance(), &LogInput::logData, this, &LogSaveBase::on_logData);
}

LogSaveBase* LogSaveBase::m_logSave = nullptr;

