#include "logsavetxt.h"

#include <qdebug.h>
#include <qthread.h>

LogSaveTxt::LogSaveTxt(QObject* parent)
    : LogSaveBase(parent)
{
    m_strLogFormat = "%1 %2 %3 %4 %5 %6";
    m_strNameFormat = "yyyy-MM-dd HH-mm-ss.log";
    m_strTimeNameFormat = "yyyy-MM-dd.log";
}

LogSaveTxt::~LogSaveTxt() {}

void LogSaveTxt::initLog()
{
    LogSaveTxt::getInstance();
}

LogSaveBase* LogSaveTxt::getInstance()
{
    if (nullptr == m_logSave)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (nullptr == m_logSave)
        {
            m_logSave = new LogSaveTxt();
        }
    }
    return m_logSave;
}

/**
 * @brief          设置日志文件保存的数据类型
 * @param type
 */
void LogSaveTxt::setFileType(FileType type)
{
    this->m_type = type;

    QMutexLocker locker(&m_mutex);
    if (Log == m_type)
    {
        m_strLogFormat = "%1 %2 %3 %4 %5 %6";
        m_strNameFormat = "yyyy-MM-dd HH-mm-ss.log";
        m_strTimeNameFormat = "yyyy-MM-dd.log";
    }
    else if (CSV == m_type)
    {
        m_strLogFormat = "%1,%2,%3,%4,%5,%6";
        m_strNameFormat = "yyyy-MM-dd HH-mm-ss.CSV";   // 这里后缀只能用大写，因为小写的会被QTime替换
        m_strTimeNameFormat = "yyyy-MM-dd.CSV";
    }
    else
    {
    }

    LogConfig::setTxtLogName("");   // 清除配置文件中的日志文件名，便于立刻替换Log/CSV文件，如果没有这一行会等待满足创建新文件的条件才会替换Log/CSV
    m_file.close();
}

void LogSaveTxt::on_logData(QtMsgType type, QString time, QString file, QString function, int line, QString msg)
{
    QString strData = "debug";
    switch (type)
    {
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

    QMutexLocker locker(&m_mutex);
    if (!openNewFile())
    {
        return;
    }

    m_out << m_strLogFormat.arg(time).arg(strData).arg(file).arg(function).arg(line).arg(msg) << "\n";
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
    if (m_file.open(QIODevice::Append | QIODevice::Text))
    {
        m_out.setDevice(&m_file);
        return true;
    }
    else
    {
        qWarning() << "打开日志文件失败";
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
    QString strName = QDateTime::currentDateTime().toString(m_strTimeNameFormat);
    if (LogConfig::txtConfig.time == 12)
    {
        if (QTime::currentTime().hour() <= 12)
        {
            strName.replace(".log", "_上.log");
            strName.replace(".CSV", "_上.CSV");
        }
        else
        {
            strName.replace(".log", "_下.log");
            strName.replace(".CSV", "_下.CSV");
        }
    }

    if (!m_file.isOpen())   // 文件未打开
    {
        if (LogConfig::txtConfig.name.isEmpty())   // 配置文件中是否有文件名
        {
            LogConfig::setTxtLogName(strName);
        }
        return openFile(LogConfig::txtConfig.name);
    }
    else
    {
        if ((LOG_PATH + strName) != m_file.fileName())   // 路径 + 新文件名 与打开的文件是否相同
        {
            LogConfig::setTxtLogName(strName);
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
    if (!m_file.isOpen())   // 文件未打开
    {
        if (LogConfig::txtConfig.name.isEmpty())
        {
            QString strName = QDateTime::currentDateTime().toString(m_strNameFormat);
            LogConfig::setTxtLogName(strName);
        }
        return openFile(LogConfig::txtConfig.name);
    }
    else
    {
        if (m_file.size() >= LogConfig::txtConfig.size * 1024 * 1024)   // 判断文件大小
        {
            m_file.close();
            QString strName = QDateTime::currentDateTime().toString(m_strNameFormat);
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
    static uint rowNum = 0;   // 文件行数
    if (!m_file.isOpen())     // 文件未打开
    {
        if (LogConfig::txtConfig.name.isEmpty())
        {
            QString strName = QDateTime::currentDateTime().toString(m_strNameFormat);
            LogConfig::setTxtLogName(strName);
        }
        if (openFile(LogConfig::txtConfig.name))
        {
            rowNum = QString(m_file.readAll()).split('\n').count();   // 获取文件行数
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        rowNum++;
        if (rowNum > LogConfig::txtConfig.rowNum)   // 判断文件行数
        {
            rowNum = 1;
            m_file.close();
            QString strName = QDateTime::currentDateTime().toString(m_strNameFormat);
            LogConfig::setTxtLogName(strName);
            return openFile(strName);
        }
        return true;
    }
}
