#include <QApplication>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qtableview.h>

/**
 * @brief  创建并打开一个QSqlite数据库，并创建一个主表、两个从表
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

    query.exec("create table grade (id int, name varchar(20))");  // 创建年级从表
    query.exec("insert into grade values(1, '一年级')");
    query.exec("insert into grade values(2, '二年级')");
    query.exec("insert into grade values(3, '三年级')");

    query.exec("create table class (id int, name varchar(20))");  // 创建班级从表
    query.exec("insert into class values(1, '(1)班')");
    query.exec("insert into class values(2, '(2)班')");
    query.exec("insert into class values(3, '(3)班')");
    query.exec("insert into class values(4, '(4)班')");

    return true;
}

/**
 * @brief        初始化设置model
 * @param model
 */
void initModel(QSqlRelationalTableModel* model)
{
    model->setTable("student");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);      // 在界面修改后不立即保存到数据库
    model->setRelation(2, QSqlRelation("grade", "id", "name"));  // 将第2列数据通过外键关联到从表
    model->setRelation(3, QSqlRelation("class", "id", "name"));
    // 设置列标题
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "姓名");
    model->setHeaderData(2, Qt::Horizontal, "年级");
    model->setHeaderData(3, Qt::Horizontal, "班级");

    model->select();   // 查询数据库数据
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!createConnection()) return -1;         // 打开数据库并创建一个主表、两个子表

    QSqlRelationalTableModel model;            // 创建一个支持使用外键的model
    initModel(&model);

    QTableView* view = new QTableView;         // 创建一个表格视图，用于显示数据库数据
    view->resize(500, 300);
    view->setWindowTitle(QString("QSql-通过QSqlRelationalTableModel类使用外键Demo - V%1").arg(APP_VERSION));
    view->setModel(&model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));  // 与默认委托不同，QSqlRelationalDelegate 为其他表的外键字段提供了一个组合框
    view->show();

    return a.exec();
}
