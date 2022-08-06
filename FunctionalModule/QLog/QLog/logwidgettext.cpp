#include "logwidgettext.h"
#include "ui_logwidgettext.h"

#include <qdebug.h>

LogWidgetText::LogWidgetText(QWidget *parent) :
    LogWidgetBase(parent),
    ui(new Ui::LogWidgetText)
{
    ui->setupUi(this);


    ui->textEdit->moveCursor(QTextCursor::End);                // 始终显示最新日志
    m_logStyle = "<span style='background:%1;'>%2 %3</span>";  // 日志的格式
}

LogWidgetText::~LogWidgetText()
{
    delete ui;
}

void LogWidgetText::on_logData(QtMsgType type, QTime time, QString file, QString function, int line, QString msg)
{
    Q_UNUSED(file)
    Q_UNUSED(function)
    Q_UNUSED(line)

    QString strColor = "transparent";
    switch (type) {
    case QtDebugMsg:
        if(!ui->check_debug->isChecked()) return;
        break;
    case QtInfoMsg:
        if(!ui->check_Info->isChecked()) return;
        strColor = "rgb(134, 210, 250)";
        break;
    case QtWarningMsg:
        if(!ui->check_warning->isChecked()) return;
        strColor = "rgb(252, 175, 62)";
      break;
    case QtCriticalMsg:
        if(!ui->check_critical->isChecked()) return;
        strColor = "rgb(250, 48, 48)";
        break;
    case QtFatalMsg:
      break;
    }

    QString strLog = m_logStyle.arg(strColor).arg(time.toString("HH:mm:ss")).arg(msg);
    ui->textEdit->append(strLog);
}

/**
 * @brief           设置日志最大显示行数
 * @param maximum
 */
void LogWidgetText::setMaxMumBlockCount(int maximum)
{
    ui->textEdit->document()->setMaximumBlockCount(maximum);
}

void LogWidgetText::on_but_clear_clicked()
{
    ui->textEdit->clear();
}
