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

    setupModel();

    QSqlTableModel* relModel = m_model->relationModel(m_index);
    ui->com_type->setModel(relModel);
    ui->com_type->setModelColumn(relModel->fieldIndex("description"));

    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(m_model);
    m_mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    m_mapper->addMapping(ui->line_name, m_model->fieldIndex("name"));
    m_mapper->addMapping(ui->text_address, m_model->fieldIndex("address"));
    m_mapper->addMapping(ui->com_type, m_index);

    connect(ui->but_previous, &QPushButton::clicked, m_mapper, &QDataWidgetMapper::toPrevious);
    connect(ui->but_next, &QPushButton::clicked, m_mapper, &QDataWidgetMapper::toNext);
    connect(m_mapper, &QDataWidgetMapper::currentIndexChanged, this, &Widget::updateButtons);

    m_mapper->toFirst();
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

    m_model = new QSqlRelationalTableModel(this);
    m_model->setTable("person");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    m_index = m_model->fieldIndex("typeid");
    m_model->setRelation(m_index, QSqlRelation("addresstype", "id", "description"));
    m_model->select();
}

void Widget::updateButtons(int row)
{
    ui->but_previous->setEnabled(row > 0);
    ui->but_next->setEnabled(row < m_model->rowCount() - 1);
}

