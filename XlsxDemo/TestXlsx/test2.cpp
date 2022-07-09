#include "test2.h"
#include "ui_test2.h"
#include <QDebug>
#include <QMetaEnum>
#include <qdir.h>
#include <qprocess.h>
#include "xlsxabstractsheet.h"

QXLSX_USE_NAMESPACE              // 添加Xlsx命名空间
#define EXCEL_NAME "test2.xlsx"  // 本demo中用到的excel文件路径文件名

Test2::Test2(QWidget *parent) :
    InterFace(parent),
    ui(new Ui::Test2)
{
    ui->setupUi(this);

    this->setWindowTitle("QXlsx中通过Document对象操作工作表Demo");
    this->setToolTip(this->windowTitle());
}

Test2::~Test2()
{
    on_but_close_clicked();
    delete ui;
}

QString Test2::getExcelName()
{
    return  EXCEL_NAME;
}

#if 0            // 使用到的函数
QStringList sheetNames() const;
bool addSheet(const QString &name = QString(),
              AbstractSheet::SheetType type = AbstractSheet::ST_WorkSheet);
bool insertSheet(int index, const QString &name = QString(),
                 AbstractSheet::SheetType type = AbstractSheet::ST_WorkSheet);
bool selectSheet(const QString &name);
bool renameSheet(const QString &oldName, const QString &newName);
bool copySheet(const QString &srcName, const QString &distName = QString());
bool moveSheet(const QString &srcName, int distIndex);
bool deleteSheet(const QString &name);
#endif

/**
 * @brief 打开Excel
 */
void Test2::on_but_open_clicked()
{
    if(!m_xlsx)
    {
        m_xlsx = new Document(EXCEL_NAME, this);        // 打开EXCEL_NAME文件，将所有数据读取到内存中，然后关闭excel文件
    }
    if(m_xlsx->load())  // 判断文件是否打开成功（也可以使用isLoadPackage）
    {
        qInfo() << "excel打开成功!";
        // 基于范围的for从容器中获取迭代器。但是因为容器不是const迭代器，所以迭代器将是非的const，这显然足以让容器分离
        const QList<QWidget*> objects =  this->findChildren<QWidget*>();
        for(auto object: objects)
        {
            object->setEnabled(true);        // 打开Excel后所有控件设置为可用状态
        }
    }
    else
    {
        qWarning() << QString("excel打开失败，可能是文件%1不存在!").arg(EXCEL_NAME);
    }
}

/**
 * @brief 释放Excel内存
 */
void Test2::on_but_close_clicked()
{
    if(m_xlsx)
    {
        delete m_xlsx;
        m_xlsx = nullptr;
        const QList<QWidget*> objects =  this->findChildren<QWidget*>();  // 禁用所有基类为QWidget的控件
        for(auto object: objects)
        {
            qDebug() << object->objectName();
            object->setEnabled(false);
        }
        ui->but_open->setEnabled(true);    // 打开Excel按键不被禁用
        qWarning() << "关闭并释放Excel";
    }
}

/**
 * @brief 查询Excel中所有的工作表（Sheet）名称
 */
void Test2::on_but_querySheet_clicked()
{
    ui->com_sheets->clear();
    ui->com_sheets->addItems(m_xlsx->sheetNames());
}

/**
 * @brief 创建不同类型的工作表
 *        QXlsx 的SheetType定义了四种类型，但是目前只支持ST_WorkSheet, ST_ChartSheet两种；
 *        如果创建了不支持的两种在Debug模式下会报错（Q_ASSERT断言），在Release下会返回失败；
 *        注意：这里有一个小bug
 *        由于还不支持ST_DialogSheet和ST_MacroSheet；
 *        如果在Release模式下如果创建了ST_DialogSheet或者ST_MacroSheet将会失败，
 *        而Q_ASSERT在Release下如果失效则会导致sheetNames添加未创建的sheetName；
 *
 */
void Test2::on_but_addSheet_clicked()
{
    QString strName = ui->line_sheetName->text();
    if(strName.isEmpty())
    {
        qWarning() << "工作表名称不能为空!";
        return;
    }
    AbstractSheet::SheetType type = (AbstractSheet::SheetType)ui->com_sheetType->currentIndex();
    bool ret = m_xlsx->addSheet(strName, type);         // 创建一个新的工作表，参数二可以省略
    qDebug() << m_xlsx->currentSheet()->sheetName();    // 创建一个新的工作表后会默认把新创建的表【设置为当前工作表】
    if(ret && m_xlsx->save())                           // 执行完操作后需要保存到原有Excel中，不保存则无效
    {
        qInfo() << QString("创建工作表：%1  %2 成功！").arg(strName).arg(type);
    }
    else
    {
        qWarning() << QString("创建工作表：%1  %2 失败！").arg(strName).arg(type);
    }
}

/**
 * @brief 在指定位置插入工作表，可设置工作表名称和类型
 */
void Test2::on_but_insert_clicked()
{
    QString strName = ui->line_sheetName->text();
    if(strName.isEmpty())
    {
        qWarning() << "工作表名称不能为空!";
        return;
    }
    AbstractSheet::SheetType type = (AbstractSheet::SheetType)ui->com_sheetType->currentIndex();
    int index = ui->spin_index->value();
    bool ret = m_xlsx->insertSheet(index, strName, type);
    if(ret && m_xlsx->save())
    {
        qInfo() << QString("在%1位置插入工作表：%2  %3 成功！").arg(index).arg(strName).arg(type);
    }
    else
    {
        qWarning() << QString("在%1位置插入工作表：%2  %3 失败！").arg(index).arg(strName).arg(type);
    }
}

/**
 * @brief 将输入名称的工作表设置为活动工作表，并写入数据，如果工作表不存在或者不是ST_WorkSheet类型则不写入
 */
void Test2::on_but_select_clicked()
{
    QString strName = ui->line_sheetName->text();
    if(strName.isEmpty())
    {
        qWarning() << "工作表名称不能为空!";
        return;
    }
    bool ret = m_xlsx->selectSheet(strName);          // 将strName设置为活动工作表
    if(ret)
    {
        qInfo() << QString("设置活动工作表【%1】成功！").arg(strName);
        AbstractSheet::SheetType type = m_xlsx->currentSheet()->sheetType();   // 获取当前工作表的类型
        if(type == AbstractSheet::ST_WorkSheet)
        {
            ret = m_xlsx->write(1, 1, "设置获取工作表！");
            if(ret && m_xlsx->save())
            {
                qInfo() << "将数据写入选择的工作表成功！";
            }
            else
            {
                qInfo() << "将数据写入选择的工作表失败！";
            }
        }
        else
        {
            qInfo() << "其它类型sheet，不写入数据";
        }
    }
    else
    {
        qWarning() << QString("设置活动工作表%1失败，可能不存在！").arg(strName);
    }
}

/**
 * @brief 重命名sheet
 */
void Test2::on_but_rename_clicked()
{
    QString strName = ui->line_sheetName->text();
    if(strName.isEmpty())
    {
        qWarning() << "工作表名称不能为空!";
        return;
    }
    QString strNewName = ui->line_newSheetName->text();
    if(strNewName.isEmpty())
    {
        qWarning() << "新的工作表名称不能为空!";
        return;
    }
    bool ret = m_xlsx->renameSheet(strName, strNewName);
    if(ret && m_xlsx->save())
    {
        qInfo() << QString("将%1重命名为%2成功！").arg(strName, strNewName);
    }
    else
    {
        qWarning() << QString("将%1重命名为%2失败！").arg(strName, strNewName);
    }
}

/**
 * @brief 将指定的strName工作表拷贝为strNewName，如果strNewName已存在则拷贝失败
 */
void Test2::on_but_copy_clicked()
{
    QString strName = ui->line_sheetName->text();
    if(strName.isEmpty())
    {
        qWarning() << "工作表名称不能为空!";
        return;
    }
    QString strNewName = ui->line_newSheetName->text();
    if(strNewName.isEmpty())
    {
        qWarning() << "新的工作表名称不能为空!";
        return;
    }
    bool ret = m_xlsx->copySheet(strName, strNewName);       // 开始拷贝工作表
    if(ret && m_xlsx->save())
    {
        qInfo() << QString("将%1拷贝为%2成功！").arg(strName, strNewName);
    }
    else
    {
        qWarning() << QString("将%1拷贝为%2失败！").arg(strName, strNewName);
    }
}

/**
 * @brief 根据输入的工作表名称，将工作表移动到指定位置，如果工作表不存在或移动到当前位置则失败
 *        移动位置从0开始，如果大于sheet总数则移动到最后位置，如果小于0则移动到最开始位置
 */
void Test2::on_but_move_clicked()
{
    QString strName = ui->line_sheetName->text();
    if(strName.isEmpty())
    {
        qWarning() << "工作表名称不能为空!";
        return;
    }
    int index = ui->spin_index->value();
    bool ret = m_xlsx->moveSheet(strName, index);        // 开始移动工作表位置
    if(ret && m_xlsx->save())
    {
        qInfo() << QString("将%1移动到%2位置成功！").arg(strName).arg(index);
    }
    else
    {
        qWarning() << QString("将%1移动到%2位置失败！").arg(strName).arg(index);
    }
}

/**
 * @brief 删除指定名称的工作表，如果指定名称的工作表不存在则返回失败
 */
void Test2::on_but_delete_clicked()
{
    QString strName = ui->line_sheetName->text();
    if(strName.isEmpty())
    {
        qWarning() << "工作表名称不能为空!";
        return;
    }
    bool ret = m_xlsx->deleteSheet(strName);        // 开始删除
    if(ret && m_xlsx->save())
    {
        qInfo() << QString("删除工作表%1成功！").arg(strName);
    }
    else
    {
        qWarning() << QString("删除工作表%1失败！").arg(strName);
    }
}

