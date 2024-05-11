#include "loginput.h"

QtMessageHandler messageHandle;

LogInput::LogInput(QObject* parent)
    : QObject(parent)
{
    LogConfig::init();
    qRegisterMetaType<QtMsgType>("QtMsgType");

    messageHandle = qInstallMessageHandler(LogInput::myMessageOutput);   // 安装日志处理函数(这里需要使用非成员函数或者静态成员函数)
}

LogInput::~LogInput()
{
    qInstallMessageHandler(0);   // 还原
}

LogInput* LogInput::m_log = nullptr;

LogInput* LogInput::getInstance()
{
    if (nullptr == m_log)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (nullptr == m_log)
        {
            m_log = new LogInput();
        }
    }
    return m_log;
}

void LogInput::myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
    case QtDebugMsg:
        break;
    case QtInfoMsg:
        break;
    case QtWarningMsg:
        break;
    case QtCriticalMsg:
        break;
    case QtFatalMsg:
        break;
    }
    if (!msg.isEmpty())
    {
        const char* file = context.file ? context.file : "";
        const char* function = context.function ? context.function : "";
        emit m_log->logData(type, QTime::currentTime().toString("HH:mm:ss"), file, function, context.line, msg);
    }
#ifdef OUT_TERMINAL
    messageHandle(type, context, msg);   // 输出到控制台
#endif
}
