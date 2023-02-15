#include "widget.h"
#include "ui_widget.h"

#include <qdir.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt使用cef加载网页简单示例 - V%1").arg(APP_VERSION));  // 设置窗口标题

    // 构建web资源的路径
#if 0        // 在线显示网页
    QString uri = "https://www.baidu.com";
#else        // 显示本地html
    QDir dir = QCoreApplication::applicationDirPath();
    QString uri = QString("file://") + QDir::toNativeSeparators(dir.filePath("hello.html"));
#endif

    // 每个QCefView的生成设置
    QCefSetting setting;
    // 设置背景色
    setting.setBackgroundColor(QColor::fromRgb(255, 255, 255));

    // 创建QCefView小部件并将其添加到布局容器
    cefViewWidget = new QCefView(uri, &setting, this);
    ui->gridLayout->addWidget(cefViewWidget);
}

Widget::~Widget()
{
    delete ui;
}

