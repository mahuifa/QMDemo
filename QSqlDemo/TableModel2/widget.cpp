#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->resize(800, 700);
    this->setWindowTitle(QString("QSql-使用QSqlTableModel显示数据库内容Demo2 - V%1").arg(APP_VERSION));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_but_connect_clicked()
{
    if(ui->but_connect->text() == "关闭数据库")
    {
        ui->but_connect->setText("连接数据库");

        m_db.close();

    }
    else
    {

        m_db = QSqlDatabase::addDatabase("QSQLITE");  // 使用数据库驱动（Qsqlite）和默认连接名称（qt_sql_default_connection）添加一个数据库
        qDebug() << QSqlDatabase::defaultConnection;           // 打印默认数据库连接名称
    #if 1
        m_db.setDatabaseName("tableModel2.db");        // 使用文件数据库（可生成数据库文件，数据一直有效）
    #else
        m_db.setDatabaseName(":memory:");       // 使用内存数据库（不会生成数据库文件，所有数据都在内存中进行操作，性能强，程序退出后数据丢失）
    #endif

        if(!m_db.open())             // 打开数据库
        {
            QMessageBox::critical(nullptr, "Error", "打开数据库失败！");
            return ;
        }

        // 如果表不存在则创建表
        if(!isTableExists("person"))
        {
            QSqlQuery query;

            // 创建一个表person，包含id、firstname、lastname三个字段
            bool ret = query.exec("create table person ("
                       "id         integer primary key,"    // 索引(自增key)，使用integer默认为自增， int不能设置主键自增
                       "firstname  varchar(20),"         // 名
                       "lastname   varchar(20))");       // 姓
            if(!ret)
            {
                qDebug() << "创建表失败：";
            }
        }

        initModel();

        ui->but_connect->setText("关闭数据库");
    }
}

/**
 * @brief        判断表是否存在
 * @param table  表名称
 * @return       true存在 false不存在
 */
bool Widget::isTableExists(const QString &table)
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
 * @brief SQL 表模型（QSqlTableModel）来编辑数据库中的信息
 */
void Widget::initModel()
{
    if(m_model)
    {
        m_model->clear();
        delete m_model;
        m_model = nullptr;
    }
    m_model = new QSqlTableModel(this, m_db);
    m_model->setTable("person");                                // 设置需要显示的数据库表
#if 1
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);    // 在界面上修改后数据立刻保存到数据库
#else
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);   // 将将编辑数据库中值的策略设置为[在调用 submitAll() 或 revertAll() 之前，所有更改都将缓存在模型中（即在界面上修改数据后不会立刻存入数据库）]
#endif
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "名称");
    m_model->setHeaderData(2, Qt::Horizontal, "姓氏");
    ui->tableView->setModel(m_model);
}

void Widget::on_but_add_clicked()
{
    QSqlQuery query;
    query.prepare("insert into person(firstname, lastname)"       // 写入数据时不需写入id字段，实现自增
                  "values (:firstname, :lastname)");
    query.bindValue(":firstname", "");
    query.bindValue(":lastname", "");
    query.exec();
    m_model->select();                                           // 获取数据库中的数据
}

void Widget::on_but_read_clicked()
{
    if(!m_model) return;
    m_model->select();                                         // 获取数据库中的数据
    ui->tableView->resizeColumnsToContents();                  // 根据表格中的内容自动调整列宽
}
