#include "crashhandler.h"
#include <qglobal.h>
#include <QDateTime>
#include <QMessageBox>

#if 1   //defined(_MSC_VER)
#    if defined(_MSC_VER) && (_MSC_VER >= 1600) && (_MSC_VER <= 1900)
#        pragma execution_character_set("utf-8")
#    endif
// clang-format off
#include <Windows.h>   // Windows.h必须放在DbgHelp.h前，否则编译会报错
#include <DbgHelp.h>
#include <qapplication.h>
#include <qdir.h>
// clang-format on

/**
 * @brief              应用程序崩溃处理程序
 * @param pException
 * @return             EXCEPTION_EXECUTE_HANDLER equ 1 表示我已经处理了异常,可以优雅地结束了
 *                     EXCEPTION_CONTINUE_SEARCH equ 0 表示我不处理,其他人来吧,于是windows调用默认的处理程序显示一个错误框,并结束(qt中会导致窗口卡死一段时间)
 *                     EXCEPTION_CONTINUE_EXECUTION equ -1 表示错误已经被修复,请从异常发生处继续执行
 */
LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
    //创建 Dump 文件
    QString path = qApp->applicationDirPath() + "/crash/";   // 文件保存路径
    QDir().mkpath(path);
    path += QString("%1_%2.dmp").arg(APP_NAME, QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss"));   // 文件名
#    ifdef UNICODE
    LPCWSTR filePath = reinterpret_cast<LPCWSTR>(path.utf16());
#    else
    LPCSTR filePath = reinterpret_cast<LPCSTR>(path.toStdString().data());
#    endif   // !UNICODE

    HANDLE hDumpFile = CreateFile(filePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hDumpFile != INVALID_HANDLE_VALUE)
    {
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, nullptr, nullptr);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(quint64(record->ExceptionCode), 16));
    QString errAdr(QString::number(quint64(record->ExceptionAddress), 16));

    QMessageBox::critical(nullptr, "不会吧，程序居然发生异常了~",
                          "<FONT size=4><div><b>对于发生的错误，表示诚挚的歉意</b><br/></div>" + QString("<div>错误代码：%1</div><div>错误地址：%2</div></FONT>").arg(errCode).arg(errAdr),
                          QMessageBox::Ok);

    return EXCEPTION_EXECUTE_HANDLER;
}

void CrashHandler::initCrashHandler()
{
    SetUnhandledExceptionFilter(static_cast<LPTOP_LEVEL_EXCEPTION_FILTER>(ApplicationCrashHandler));   // 使用win API注册异常处理函数
}
#else
void CrashHandler::initCrashHandler() {}
#endif
