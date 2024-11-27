#include "logsavebase.h"

#include "loginput.h"
#include <qmutex.h>
#include <qthread.h>
#include <QDir>

LogSaveBase::LogSaveBase(QObject* parent)
    : QObject(parent)
{
    // 将数据保存放入线程中
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();

    QDir dir;
    if (!dir.exists(LOG_PATH))
    {
        dir.mkpath(LOG_PATH);
    }
    connect(LogInput::getInstance(), &LogInput::logData, this, &LogSaveBase::on_logData, Qt::QueuedConnection);
}

LogSaveBase::~LogSaveBase()
{
    m_thread->quit();
    m_thread->wait();
    disconnect(LogInput::getInstance(), &LogInput::logData, this, &LogSaveBase::on_logData);
}

LogSaveBase* LogSaveBase::m_logSave = nullptr;
