#include "logsavetxt.h"
#include "head.h"

#include <qdebug.h>

LogSaveTxt::LogSaveTxt(QObject *parent) : LogSaveBase(parent)
{
}

LogSaveTxt::~LogSaveTxt()
{

}

LogSaveBase *LogSaveTxt::getInstance()
{
    if(nullptr == m_logSave)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(nullptr == m_logSave)
        {
            m_logSave = new LogSaveTxt();
        }
    }
    return m_logSave;
}

void LogSaveTxt::on_logData(QtMsgType type, QTime time, QString file, QString function, int line, QString msg)
{
    if(!openNewFile())
    {
        return;
    }

    QString strData = "debug";
    switch (type) {
    case QtDebugMsg:
        strData = "debug";
        return;
    case QtInfoMsg:
        strData = "info";
        break;
    case QtWarningMsg:
        strData = "warning";
      break;
    case QtCriticalMsg:
        strData = "critical";
        break;
    case QtFatalMsg:
        strData = "fatal";
      break;
    }

    strData += QString(10 - strData.count(), ' ');
    QString strLog = QString("%1 %2 %3 %4 %5 %6").arg(time.toString("HH:mm:ss")).arg(strData).arg(file).arg(function).arg(line).arg(msg);
    m_out << strLog <<"\n";
    m_out.flush();
}

/**
 * @brief         打开文件
 * @param name    打开的文件名
 * @return        true：打开成功 false：打开失败
 */
bool LogSaveTxt::openFile(QString name)
{
    m_file.setFileName(QString("%1%2").arg(LOG_PATH).arg(name));
    if(m_file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
    {
        m_out.setDevice(&m_file);
        return true;
    }
    else
    {
        qWarning() <<"打开日志文件失败";
        return false;
    }
}

/**
 * @brief    打开日志文件或创建新的日志文件
 * @return   true：文件打开成功 false：文件打开失败
 */
bool LogSaveTxt::openNewFile()
{
    switch (LogConfig::txtConfig.relyMode)
    {
    case Time:
    {
        return relyTime();
    }
    case Size:
    {
        return relySize();
    }
    case RowNum:
    {
        return relyRowNum();
    }
    default:
        return false;
    }
}

/**
 * @brief   以时间为依据创建新的日志文件
 * @return
 */
bool LogSaveTxt::relyTime()
{
    QString strName = QDateTime::currentDateTime().toString("yyyy-MM-dd.log");
    if(LogConfig::txtConfig.time == 12)
    {
        if(QTime::currentTime().hour() <= 12)
        {
            strName.replace(".log", "_上.log");
        }
        else
        {
            strName.replace(".log", "_下.log");
        }
    }

    if(!m_file.isOpen())                    // 文件未打开
    {
        return openFile(strName);
    }
    else
    {
        if(strName != m_file.fileName())
        {
            m_file.close();
            return openFile(strName);
        }
        return true;
    }
}

/**
 * @brief  以日志文件大小为依据创建新的文件
 * @return
 */
bool LogSaveTxt::relySize()
{
    if(!m_file.isOpen())                    // 文件未打开
    {
        if(LogConfig::txtConfig.name.isEmpty())
        {
            QString strName = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss.log");
            LogConfig::setTxtLogName(strName);
        }
        return openFile(LogConfig::txtConfig.name);
    }
    else
    {
        if(m_file.size() >= LogConfig::txtConfig.size * 1024 * 1024)   // 判断文件大小
        {
            m_file.close();
            QString strName = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss.log");
            LogConfig::setTxtLogName(strName);
            return openFile(strName);
        }
        return true;
    }
}

/**
 * @brief   以日志行数为依据创建新的文件
 * @return
 */
bool LogSaveTxt::relyRowNum()
{
    if(!m_file.isOpen())                    // 文件未打开
    {
        if(LogConfig::txtConfig.name.isEmpty())
        {
            QString strName = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss.log");
            LogConfig::setTxtLogName(strName);
        }
        if(openFile(LogConfig::txtConfig.name))
        {
            QByteArray arr = m_file.readAll();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(m_file.size() >= LogConfig::txtConfig.size * 1024 * 1024)   // 判断文件大小
        {
            m_file.close();
            QString strName = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss.log");
            LogConfig::setTxtLogName(strName);
            return openFile(strName);
        }
        return true;
    }
}


