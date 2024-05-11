/******************************************************************************
* @文件名     logsavetxt.h
* @功能       将日志保存到txt中
* @开发者     mhf
* @时间      2021/11/20
* @备注
*****************************************************************************/
#ifndef LOGSAVETXT_H
#define LOGSAVETXT_H

#include "logsavebase.h"

#include "logconfig.h"
#include <QFile>
#include <QTextStream>

class LogSaveTxt : public LogSaveBase
{
    Q_OBJECT
    Q_PROPERTY(FileType m_type READ fileType WRITE setFileType)

public:
    enum FileType   // 日志保存的TxT文件类型
    {
        Log,   // 日志保存到纯txt文本中
        CSV,   // 日志保存为csv文件，方便查看
    };

public:
    static void initLog();               // 初始化日志保存功能
    static LogSaveBase* getInstance();   // 获取单例对象
    void setFileType(FileType type);

    FileType fileType() { return m_type; }

protected:
    void on_logData(QtMsgType type, QString time, QString file, QString function, int line, QString msg) override;
    bool openNewFile() override;
    bool relyTime();
    bool relySize();
    bool relyRowNum();
    bool openFile(QString name);

private:
    explicit LogSaveTxt(QObject* parent = nullptr);
    ~LogSaveTxt();

signals:

private:
    QFile m_file;
    QTextStream m_out;
    FileType m_type;
    QMutex m_mutex;
    QString m_strLogFormat;        // 保存的日志内容格式
    QString m_strNameFormat;       // 保存的日志文件名称格式
    QString m_strTimeNameFormat;   // 保存的日志文件名称格式
};

#endif   // LOGSAVETXT_H
