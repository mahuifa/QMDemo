#include <QCoreApplication>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>

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

    // 创建一个主表person，包含id、name、grade、class四个字段
    query.exec("create table student ("
               "id         int         primary key,"    // 索引
               "name       varchar(20),"                // 姓名
               "grade      int,"                        // 年级
               "class      int )");                     // 班级

    query.exec("insert into student values(1, '大毛', 3, 2)");       // 插入学生数据（大毛、三年级2班）
    query.exec("insert into student values(2, '二毛', 3, 1)");
    query.exec("insert into student values(3, '三毛', 2, 3)");
    query.exec("insert into student values(4, '小毛', 1, 4)");

    query.exec("create table grade (id int, name varchar(20))");  // 创建年级子表
    query.exec("insert into grade values(1, '一年级')");
    query.exec("insert into grade values(2, '二年级')");
    query.exec("insert into grade values(3, '三年级')");

    query.exec("create table class (id int, name varchar(20))");  // 创建班级子表
    query.exec("insert into class values(1, '(1)班')");
    query.exec("insert into class values(2, '(2)班')");
    query.exec("insert into class values(3, '(3)班')");
    query.exec("insert into class values(4, '(4)班')");

    return true;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(!createConnection()) return -1;


    return a.exec();
}
