#include "logwidgettext.h"
#include "ui_logwidgettext.h"

#include <qdebug.h>

LogWidgetText::LogWidgetText(QWidget* parent)
    : LogWidgetBase(parent)
    , ui(new Ui::LogWidgetText)
{
    ui->setupUi(this);

    setMaxMumBlockCount(50);                                         // 默认最大显示100行（如果显示行数过多会影响性能）
    ui->textEdit->moveCursor(QTextCursor::End);                      // 始终显示最新日志
    m_logStyle = "<span style='background:%1;'>%2 %3</span><br/>";   // 日志的格式

    // 200毫秒显示一次，防止频繁刷新导致卡顿
    m_timer.start(2);
    connect(&m_timer, &QTimer::timeout, this, &LogWidgetText::on_showLog);
}

LogWidgetText::~LogWidgetText()
{
    delete ui;
}

void LogWidgetText::on_logData(QtMsgType type, QString time, QString file, QString function, int line, QString msg)
{
    Q_UNUSED(file)
    Q_UNUSED(function)
    Q_UNUSED(line)

    QString strColor = "transparent";
    switch (type)
    {
    case QtDebugMsg:
        if (!ui->check_debug->isChecked())
            return;
        break;
    case QtInfoMsg:
        if (!ui->check_Info->isChecked())
            return;
        strColor = "rgb(134, 210, 250)";
        break;
    case QtWarningMsg:
        if (!ui->check_warning->isChecked())
            return;
        strColor = "rgb(252, 175, 62)";
        break;
    case QtCriticalMsg:
        if (!ui->check_critical->isChecked())
            return;
        strColor = "rgb(250, 48, 48)";
        break;
    case QtFatalMsg:
        break;
    }

    QMutexLocker locker(&m_mutex);
    m_strBuf.append(m_logStyle.arg(strColor).arg(time).arg(msg));
}

/**
 * @brief 定时将数据显示到控件中
 */
void LogWidgetText::on_showLog()
{
    QMutexLocker locker(&m_mutex);
    if (m_strBuf.isEmpty())
        return;

    ui->textEdit->append(m_strBuf);
    m_strBuf.clear();
}

/**
 * @brief           设置日志最大显示行数
 * @param maximum
 * TODO: 表示每次append为一块，而不是实际行数，所以如果使用定时器将缓存的数据显示则会超过指定行数
 */
void LogWidgetText::setMaxMumBlockCount(int maximum)
{
    ui->textEdit->document()->setMaximumBlockCount(maximum);
}

void LogWidgetText::on_but_clear_clicked()
{
    ui->textEdit->clear();
}
