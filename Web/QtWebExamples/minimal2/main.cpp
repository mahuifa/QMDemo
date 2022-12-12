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

    // setHtml内部调用的就是【setContent(html.toUtf8(), QStringLiteral("text/html;charset=UTF-8"), baseUrl);】
#if DEMO == 0
    QString  html("<h1 align='center' style='font-size:80px; color:red;'>hello </h1>");
    view.setHtml(html);    // 直接在QWebEngineView中显示Html代码

#elif DEMO == 1
    // 百度搜索界面的图标地址 https://www.baidu.com/img/PCtm_d9c8750bed0b3c7d089fa7d55720d6cf.png
    // setHtml使用参数2 (baseUrl)后，可以在html代码中使用【相对路径】引用baseUrl中的资源
    QString html("<img src='/img/PCtm_d9c8750bed0b3c7d089fa7d55720d6cf.png'/>");   // 注意：字符串html不能超过2MB
    view.setHtml(html, QUrl("https://www.baidu.com"));

#elif DEMO == 2
    QString  html("<h1 align='center' style='font-size:80px; color:red;'>hello </h1>");
    view.setContent(html.toUtf8(), "text/html; charset=UTF-8");
#endif

    view.resize(1024, 750);
    view.show();

    return a.exec();
}
