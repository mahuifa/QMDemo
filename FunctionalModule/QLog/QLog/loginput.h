/******************************************************************************
* @文件名     loginput.h
* @功能      拦截QDebug日志信息的单例类，并通过信号发送出去
*
* @开发者     mhf
* @邮箱      1603291350@qq.com
* @时间      2022/03/27
* @备注
*****************************************************************************/
#ifndef LOGINPUT_H
#define LOGINPUT_H

#include "logconfig.h"
#include "logsavetxt.h"
#include <QObject>
#include <QTime>

class LogInput : public QObject
{
    Q_OBJECT
public:
    static LogInput* getInstance();   // 获取单例对象
    static void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private:
    explicit LogInput(QObject* parent = nullptr);
    ~LogInput();

signals:
    /**
     * @brief           日志信息
     * @param type      日志级别
     * @param time      日志时间
     * @param file      日志来源文件
     * @param function  日志来源函数
     * @param line      日志来源行
     * @param msg       日志信息
     */
    void logData(QtMsgType type, QString time, QString file, QString function, int line, QString msg);

private:
    static LogInput* m_log;
};

#endif   // LOGINPUT_H
