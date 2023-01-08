#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMetaEnum>
#include <QStandardPaths>
#include <QPair>
#include <QList>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Qt使用路径的方式和注意使用 V%1").arg(APP_VERSION));

    QList<QPair<QString, QString>> paths;
    paths << QPair<QString, QString>("QDir::currentPath()", QDir::currentPath());    // 返回应用程序当前目录的绝对路径
    paths << QPair<QString, QString>("QDir::homePath()", QDir::homePath());          // 返回用户主目录的绝对路径
    paths << QPair<QString, QString>("QDir::rootPath()", QDir::rootPath());          // 返回根目录的绝对路径

    // Windows下创建一个文件夹快捷方式或linux下创建一个文件夹链接，将快捷方式路径写入下列QDir
    QDir dir("./桌面文件夹 - 快捷方式");   // 指定一个路径
    paths << QPair<QString, QString>("QDir::path()", dir.path());                    // 返回输入路径，返回的路径可以是绝对路径或相对路径
    paths << QPair<QString, QString>("QDir::absolutePath()", dir.absolutePath());    // 返回绝对路径(不区分是不是快捷方式/linux链接 ln)
    // 如果是真实路径，效果和absolutePath()相同，如果是windows快捷方式则返回空，如果是linux软硬链接则返回路径地址的实际路径
    paths << QPair<QString, QString>("QDir::canonicalPath()", dir.canonicalPath());
    paths << QPair<QString, QString>("QCoreApplication::applicationDirPath()", QCoreApplication::applicationDirPath());  // 返回包含应用程序可执行文件的目录
    paths << QPair<QString, QString>("qApp->applicationDirPath()", qApp->applicationFilePath());      // 返回应用程序可执行文件的文件路径

    // Windows下创建一个文件快捷方式或linux下创建一个文件链接，将快捷方式路径写入下列QDir
    QFileInfo info("./桌面文件夹 - 快捷方式/1.xlsx");
    // 如果是真实路径，效果和absolutePath()相同，如果是windows快捷方式则返回空，如果是linux软硬链接则返回路径地址的实际路径
    paths << QPair<QString, QString>("QFileInfo::canonicalPath()", info.canonicalPath());
    paths << QPair<QString, QString>("QFileInfo::canonicalFilePath()", info.canonicalFilePath());
    paths << QPair<QString, QString>("QFileInfo::absolutePath()", info.absolutePath());             // 返回输入文件的绝对路径（不包含文件名）
    paths << QPair<QString, QString>("QFileInfo::absoluteFilePath()", info.absoluteFilePath());     // 返回输入文件的绝对路径
    paths << QPair<QString, QString>("QFileInfo::filePath()", info.filePath());                     // 返回文件名，包括路径（可以是绝对路径或相对路径）
    paths << QPair<QString, QString>("QFileInfo::path()", info.path());                             // 返回文件的路径。这不包括文件名

    paths << QPair<QString, QString>("", "--------返回给定位置类型的本地化显示名称--------");
    QMetaEnum m = QMetaEnum::fromType<QStandardPaths::StandardLocation>();
    for(int i=0; i < m.keyCount(); ++i)
    {
        QStandardPaths::StandardLocation location = QStandardPaths::StandardLocation(m.value(i));
        // 返回给定位置类型的本地化显示名称，如果找不到相关位置，则返回空QString。
        paths << QPair<QString, QString>(m.valueToKey(m.value(i)), QStandardPaths::displayName(location));
    }

    paths << QPair<QString, QString>("", "--------返回类型文件应写入的目录--------");
    for(int i=0; i < m.keyCount(); ++i)
    {
        QStandardPaths::StandardLocation location = QStandardPaths::StandardLocation(m.value(i));
        // 返回类型文件应写入的目录，如果无法确定位置，则返回空字符串。
        paths << QPair<QString, QString>(m.valueToKey(m.value(i)), QStandardPaths::writableLocation(location));
    }

    for(int i = 0; i < paths.count(); i++)
    {
        ui->tableWidget->insertRow(i);		//按需求加行
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(paths.at(i).first));   // 插入第一列数据
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(paths.at(i).second));  // 插入第二列数据
    }

    //自适应宽度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

Widget::~Widget()
{
    delete ui;
}
