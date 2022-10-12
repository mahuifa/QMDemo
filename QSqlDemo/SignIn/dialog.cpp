#include "dialog.h"
#include "ui_dialog.h"

#include <QSqlQuery>
#include <qmessagebox.h>
#include <QDebug>



bool Dialog::m_loggedIn = false;                           // 登录状态
QString Dialog::m_userName;                                // 登录的用户名
UserBackstage::UserType Dialog::m_userType;                // 用户类型

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->setWindowTitle("登录");

    m_db = QSqlDatabase::addDatabase("QSQLITE");  // 使用数据库驱动（Qsqlite）和默认连接名称（qt_sql_default_connection）添加一个数据库
    m_db.setDatabaseName("SignIn.db");        // 使用文件数据库（可生成数据库文件，数据一直有效）

    if(!m_db.open())             // 打开数据库
    {
        QMessageBox::critical(nullptr, "Error", "打开数据库失败！");
        return ;
    }

    // 创建一个用于保存用户信息的表
    if(!isTableExists("User"))
    {
        QSqlQuery query;

        // 创建一个表person，包含id、firstname、lastname三个字段
        bool ret = query.exec("create table User ("
                   "id         integer primary key,"    // 索引(自增key)
                   "userName   varchar(20),"            // 用户名
                   "password   varchar(20),"            // 密码
                   "type       int)");                  // 用户类型
        if(!ret)
        {
            qDebug() << "创建表失败：";
        }
        else
        {
            QSqlQuery query;
            query.prepare("insert into User(userName, password, type)"
                          "values (:userName, :password, :type)");
            query.bindValue(":userName", "root");
            query.bindValue(":password", "123456");
            query.bindValue(":type", int(UserBackstage::Root));
            query.exec();
        }
    }
}

Dialog::~Dialog()
{
    if(m_db.isOpen())
    {
        m_db.close();       // 关闭数据库
    }
    delete ui;
}

/**
 * @brief   返回登录状态
 * @return  true登录 false未登录
 */
bool Dialog::loggedIn()
{
    return m_loggedIn;
}

/**
 * @brief   返回登录的用户名
 * @return  如果没有则为空
 */
QString Dialog::userName()
{
    return m_userName;
}

/**
 * @brief   返回登录的用户类型
 * @return
 */
UserBackstage::UserType Dialog::type()
{
    return m_userType;
}

/**
 * @brief         判断表是否存在
 * @param table
 * @return
 */
bool Dialog::isTableExists(const QString &table)
{
    QSqlQuery query;
    QString sql = QString("select * from sqlite_master where name = '%1';").arg(table);  // 查询sqlite_master表中是否存在表名
    if(query.exec(sql))
    {
        return query.next();
    }
    return false;
}

/**
 * @brief 登录
 */
void Dialog::on_but_signIn_clicked()
{
    QString userName = ui->line_user->text().trimmed();
    QString password = ui->line_password->text().trimmed();
    if(userName.isEmpty())
    {
        QMessageBox::about(this, "注意！", "用户名不能为空");
        return;
    }
    if(password.isEmpty())
    {
        QMessageBox::about(this, "注意！", "用户密码不能为空");
        return;
    }

    // 从数据库中查询用户和密码，完成登录功能
    QSqlQuery query;
    QString sql = QString("select * from User where userName = '%1';").arg(userName);  // 查询用户
    if(query.exec(sql))
    {
        if(query.next())   // true则用户存在
        {
            if(password == query.value("password").toString())   // 密码相等
            {
                m_userName = userName;
                m_userType = UserBackstage::UserType(query.value("type").toInt());
                m_loggedIn = true;
                this->close();
            }
            else
            {
                QMessageBox::about(this, "注意", "输入密码错误！");
            }
        }
        else
        {
            QMessageBox::about(this, "注意", "用户不存在！");
        }
    }
    else
    {
        QMessageBox::about(this, "注意", "sql指令执行失败！");
    }
}
