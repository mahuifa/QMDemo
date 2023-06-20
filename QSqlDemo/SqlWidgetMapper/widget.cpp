#include "widget.h"
#include "ui_widget.h"

#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <QtSql/qsqlrelationaldelegate.h>
#include <QtSql/qsqlrelationaltablemodel.h>

#include <QDataWidgetMapper>
#include <qmessagebox.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->resize(800, 700);

    this->setWindowTitle(QString("QSql-使用QDataWidgetMapper将数据库内容映射到控件上 - V%1").arg(APP_VERSION));
    setupModel();

    QSqlTableModel* relModel = m_model->relationModel(m_index);        // 返回一个用于访问列是外键的表
    ui->com_type->setModel(relModel);                                  // 将外键访问的列model传入下拉框
    ui->com_type->setModelColumn(relModel->fieldIndex("description")); // 设置需要显示model中哪一列

    m_mapper = new QDataWidgetMapper(this);   // 可用于将模型数据映射到小部件，每次当前索引发生变化时，每个小部件都会通过映射时指定的属性使用来自模型的数据进行更新。
    m_mapper->setModel(m_model);
    m_mapper->setItemDelegate(new QSqlRelationalDelegate(this)); // 与默认委托不同，QSqlRelationalDelegate 为其他表的外键字段提供了一个组合框
    m_mapper->addMapping(ui->line_name, m_model->fieldIndex("name"));        // 在小部件和模型中的节之间添加映射
    m_mapper->addMapping(ui->text_address, m_model->fieldIndex("address"));
    m_mapper->addMapping(ui->com_type, m_index);

    connect(ui->but_previous, &QPushButton::clicked, m_mapper, &QDataWidgetMapper::toPrevious);
    connect(ui->but_next, &QPushButton::clicked, m_mapper, &QDataWidgetMapper::toNext);
    connect(m_mapper, &QDataWidgetMapper::currentIndexChanged, this, &Widget::updateButtons);

    m_mapper->toFirst();     // 显示model第一行数据
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::setupModel
 */
void Widget::setupModel()
{
    // 创建并打开数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
#if 0
    db.setDatabaseName("test.db");        // 使用文件数据库（可生成数据库文件，数据一直有效）
#else
    db.setDatabaseName(":memory:");       // 使用内存数据库（不会生成数据库文件，所有数据都在内存中进行操作，性能强，程序退出后数据丢失）
#endif
    if(!db.open())
    {
        QMessageBox::critical(nullptr, "注意！", "打开数据库失败！", QMessageBox::Cancel);
        return;
    }

    QSqlQuery query;   // 创建一个用于执行Sql语句的对象
    query.exec("create table person ("           // 创建表
               "id       int primary  key,"
               "name     varchar(20),"
               "address  varchar(200),"
               "typeid   int)");
    query.exec("insert into person values(1, '张三', '四川<br/>成都', 101)");   // 插入数据
    query.exec("insert into person values(2, '李四', '湖北<br/>武汉', 102)");
    query.exec("insert into person values(3, '王五', '贵州<br/>贵阳', 103)");
    query.exec("insert into person values(4, '唐僧', '东土<br/>大唐', 101)");

    // 创建一个表用于保存主表中人物的地址类型
    query.exec("create table addresstype("
               "id int,"
               "description varchar(20))");
    query.exec("insert into addresstype values(101, '家庭')");
    query.exec("insert into addresstype values(102, '工作')");
    query.exec("insert into addresstype values(103, '其它')");

    m_model = new QSqlRelationalTableModel(this);               // 创建一个支持外键的表格model
    m_model->setTable("person");                                // 设置主表名称
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);   // 设置修改数据后不自动保存到数据库

    m_index = m_model->fieldIndex("typeid");                    // 返回字段typeid的索引
    m_model->setRelation(m_index, QSqlRelation("addresstype", "id", "description"));  // 设置通过外键关联从表
    m_model->select();                                          // 查询数据库数据
}

/**
 * @brief       根据当前索引修改按键状态
 * @param row   当前查询数据库表的索引
 */
void Widget::updateButtons(int row)
{
    ui->but_previous->setEnabled(row > 0);
    ui->but_next->setEnabled(row < m_model->rowCount() - 1);
}

