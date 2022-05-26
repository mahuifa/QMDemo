#include "widget.h"

#include <QApplication>
#include <QSqlQuery>
#include <qmessagebox.h>
#include <qsqldatabase.h>

/**
 * @brief  创建并打开一个QSqlite数据库，并创建一个测试表person，同时默认创建5组数据
 * @return
 */
bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");  // 使用数据库驱动（Qsqlite）和默认连接名称（qt_sql_default_connection）添加一个数据库
//    qDebug() << QSqlDatabase::defaultConnection;           // 打印默认数据库连接名称
#if 1
    db.setDatabaseName("test.db");        // 使用文件数据库（可生成数据库文件，数据一直有效）
#else
    db.setDatabaseName(":memory:");       // 使用内存数据库（不会生成数据库文件，所有数据都在内存中进行操作，性能强，程序退出后数据丢失）
#endif

    if(!db.open())             // 打开数据库
    {
        QMessageBox::critical(nullptr, "Error", "打开数据库失败！");
        return false;
    }

    QSqlQuery query;          // 创建一个用于执行和操作Sql语句的对象

    // 创建一个表person，包含id、firstname、lastname三个字段
    query.exec("create table person ("
               "id         int primary  key,"    // 索引
               "firstname  varchar(20),"         // 名
               "lastname   varchar(20))");       // 姓

    /****************** 向表中插入数据 ************************/

    // 插入方式一：直接插入数据
    query.exec("insert into person values(1, '悟空', '孙')");       // INSERT INTO 语法1（为表中所有项插入数据）

    // 插入方式二：使用命名占位符的[命名]绑定
    query.prepare("insert into person(id, firstname, lastname)"       // insert into 语法2
                  "values (:id, :firstname, :lastname)");
    query.bindValue(":id", 2);
    query.bindValue(":firstname", "悟净");
    query.bindValue(":lastname", "沙");
    query.exec();

    // 插入方式三：使用命名占位符的[位置]绑定
    query.prepare("insert into person(id, firstname, lastname)"       // insert into 语法2
                  "values (:id, :firstname, :lastname)");
    query.bindValue(0, 3);
    query.bindValue(1, "八戒");
    query.bindValue(2, "猪");
    query.exec();

    // 插入方式四：使用位置占位符绑定值（版本 1）
    query.prepare("insert into person(id, firstname, lastname)"       // insert into 语法2
                  "values (?, ?, ?)");
    query.bindValue(0, 4);
    query.bindValue(1, "白龙");
    query.bindValue(2, "小");
    query.exec();

    // 插入方式五：使用位置占位符绑定值（版本 2）
    query.prepare("insert into person(id, firstname, lastname)"       // insert into 语法2
                  "values (?, ?, ?)");
    query.addBindValue(5);                                           // 使用位置值绑定时，将值val添加到值列表中。addBindValue（）调用的顺序决定了在准备好的查询中将值绑定到哪个占位符
    query.addBindValue("三藏");
    query.addBindValue("唐");
    query.exec();

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!createConnection()) return -1;    // 创建并打开一个数据库

    Widget w;
    w.show();
    return a.exec();
}
