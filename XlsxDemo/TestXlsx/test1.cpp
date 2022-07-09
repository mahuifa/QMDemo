#include "test1.h"
#include "ui_test1.h"
#include <QDebug>
#include <QProcess>
#include <qdir.h>

QXLSX_USE_NAMESPACE            // 添加Xlsx命名空间

#define EXCEL_NAME "test1.xlsx"  // 本demo中用到的excel文件路径文件名

Test1::Test1(QWidget *parent) :
    InterFace(parent),
    ui(new Ui::Test1)
{
    ui->setupUi(this);
    this->setWindowTitle("QXlsx基本读写Excel");
    this->setToolTip(this->windowTitle());
}

Test1::~Test1()
{
    on_but_close_clicked();
    delete ui;
}

QString Test1::getExcelName()
{
    return EXCEL_NAME;
}

/**
 * @brief 创建一个excel文件
 */
void Test1::on_but_init_clicked()
{
    Document xlsx;                      // 初始化后默认有一个sheet1
//    doc.addSheet("第一个sheet", AbstractSheet::ST_WorkSheet);  // 添加第一个sheet，如果不添加则会有一个默认的
    bool ret = xlsx.saveAs(EXCEL_NAME); // 保存到EXCEL_NAME，如果已经存在则覆盖
    if(ret)
    {
        qInfo() << "创建excel成功！";
    }
    else
    {
        qWarning() << "创建excel失败！";
    }
}

/**
 * @brief 打开文件并判断是否打开成功
 */
void Test1::on_but_open_clicked()
{
    if(!m_xlsx)
    {
        m_xlsx = new Document(EXCEL_NAME, this);        // 打开EXCEL_NAME文件，将所有数据读取到内存中，然后关闭excel文件
    }
    if(m_xlsx->load())  // 判断文件是否打开成功（也可以使用isLoadPackage）,如果文件不存在则为false
    {
        qInfo() << "excel打开成功!";

        ui->but_write->setEnabled(true);
        ui->but_recv->setEnabled(true);
        ui->but_saveAs->setEnabled(true);
        ui->but_close->setEnabled(true);
    }
    else
    {
        qWarning() << "excel打开失败!";
    }
}

/**
 * @brief 使用两种不同的方式将数据写入excel，未指定sheet则默认为sheet1
 */
void Test1::on_but_write_clicked()
{
#if 0
    m_xlsx->write("A2", 123);         // 从A1开始
    m_xlsx->write("B2", 0.001);
    m_xlsx->write("C2", "abc");
    m_xlsx->write("D2", true);
    m_xlsx->write("E2", "你好");
#else
    m_xlsx->write(1, 1, 456);        // 从1，1开始
    m_xlsx->write(1, 2, 0.002);
    m_xlsx->write(1, 3, "aaa");
    m_xlsx->write(1, 4, false);
    m_xlsx->write(1, 5, "阿斯蒂芬");
#endif
    if(m_xlsx->save())
    {
        qInfo() << "数据写入成功！";
    }
    else
    {
        qWarning() << "数据写入失败！";
    }
}

/**
 * @brief 读取excel中所有数据
 */
void Test1::on_but_recv_clicked()
{
    int rowLen = m_xlsx->dimension().rowCount();           // 获取最大行数
    int columnLen = m_xlsx->dimension().columnCount();     // 获取最大列数
    for(int i = 1; i <= rowLen; i++)                       // 遍历每一行
    {
        QString data = QString("第%1行：").arg(i);
        for(int j = 1; j <= columnLen; j++)                // 遍历每一个单元格(列)
        {
#if 1
            QVariant value = m_xlsx->read(i, j);                                         // 通过单元格行号、列号读取数据
#else
            QVariant value = m_xlsx->read(QString("%1%2").arg((char)(64 + i)).arg(j));   // 通过单元格引用读取数据
#endif
            if(!value.isNull())
            {
                data.append(value.toString()).append(" ");
            }
            else
            {
                data.append("NULL ");
            }
        }
        qInfo() << data;
    }

}

/**
 * @brief 将数据另存到2.xlsx
 */
void Test1::on_but_saveAs_clicked()
{
    if(m_xlsx->saveAs("./2.xlsx"))
    {
        qInfo() << "另存为2.xlsx";
    }
    else
    {
        qWarning() << "文件另存为失败！";
    }
}

/**
 * @brief 释放数据
 */
void Test1::on_but_close_clicked()
{
    if(m_xlsx)
    {
        delete m_xlsx;
        m_xlsx = nullptr;
        ui->but_write->setEnabled(false);
        ui->but_recv->setEnabled(false);
        ui->but_saveAs->setEnabled(false);
        ui->but_close->setEnabled(false);
    }
}
