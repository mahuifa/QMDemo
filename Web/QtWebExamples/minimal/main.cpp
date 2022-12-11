#include <QApplication>
#include <QDir>
#include <QWebEngineView>

#define DEMO 0    // 通过修改DEMO值 为0、1、2使用不同的演示代码

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);  // 在支持的平台上启用Qt中的高DPI缩放
    QApplication a(argc, argv);

    QWebEngineView view;  // 创建一个用于加载显示、编辑Web页面的Widget
    view.setWindowTitle(QString("QWebEngineView最小示例 - V%1").arg(APP_VERSION));  // 设置窗口标题
    // 使用setUrl和load都一样
#if DEMO == 0
    view.setUrl(QUrl("https://www.baidu.com"));      // 网址：原文中使用QStringLiteral("https://www.qt.io")创建，但是这里只是创建一次，没必要
#elif DEMO == 1
    view.setUrl(QUrl("qrc:/hello.html"));            // 如果html文件在资源文件中，这里【qrc】不能少
#elif DEMO == 2
    view.setUrl(QDir("hello.html").absolutePath());  // 如果html文件不在资源文件中，这里需要使用【绝对路径】
#endif

    view.resize(1024, 750);
    view.show();

    return a.exec();
}
