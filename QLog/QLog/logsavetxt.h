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

#include <QFile>
#include <QTextStream>
#include "logconfig.h"

class LogSaveTxt : public LogSaveBase
{
    Q_OBJECT
public:
    static LogSaveBase* getInstance();           // 获取单例对象
    void on_logData(QtMsgType type, QTime time, QString file, QString function, int line, QString msg) override;

protected:
    bool openNewFile() override;
    bool relyTime();
    bool relySize();
    bool relyRowNum();
    bool openFile(QString name);

private:
    explicit LogSaveTxt(QObject *parent = nullptr);
    ~LogSaveTxt();

signals:

private:
    QFile m_file;
    QTextStream m_out;

};

#endif // LOGSAVETXT_H
