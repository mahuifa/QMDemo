#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("Qt使用内置库读写Json简单示例 - V%1").arg(APP_VERSION));  // 设置窗口标题
}

Widget::~Widget()
{
    delete ui;
}


/**
 * @brief 生成json
 */
void Widget::on_but_create_clicked()
{
    // json数组添加数据方式1
    QJsonArray array = {1, 2, 3, 4, "123"};

    // json数组添加数据方式2
    array.append(QJsonValue(true));        // 添加bool 数据
    array.append(QJsonValue(1.23));        // 添加double 数据
    array.append(QJsonValue(123));         // 添加int数据
    array.append(QJsonValue("abc"));       // 添加Qstring数据
    QJsonArray array1 = {111, 222, 333};
    array.append(QJsonValue(array1));      // 添加数组数据
    QJsonObject obj1   // json对象添加【键值对】方式1
    {
        {"键值1", 1},
        {"键值2", 2}
    };
    array.append(QJsonValue(obj1));        // 添加JSON对象数据

    QJsonObject obj2;
    // json对象添加【键值对】方式2
    obj2.insert("键值0", array);

    m_doc.setObject(obj2);
    QString strJaon(m_doc.toJson(QJsonDocument::Indented).constData());   // json转Qstring，indented表示转为可读格式（有换行、缩进）
    ui->textEdit->append("<------------- 生成json ------------->");
    ui->textEdit->append(strJaon);
}

/**
 * @brief  将json数据保存到文件中
 */
void Widget::on_but_save_clicked()
{
    if(m_doc.isEmpty()) return;    // 如果没有json数据则不保存 isEmpty和isNull一样

    QString strName = QString("./%1.json").arg(QTime::currentTime().toString("HH-mm-ss"));
    QString path = QFileDialog::getSaveFileName(this, "json数据保存到", strName, "Json文件 (*.json)");
    if(path.isEmpty()) return;

    // 将json数据保存到文件
    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(m_doc.toJson(QJsonDocument::Indented));
        m_doc = QJsonDocument();   // 清空
        file.close();
    }
    else
    {
        qWarning() << "保存失败！";
    }
}

/**
 * @brief 从文件中读取json数据并解析
 */
void Widget::on_but_parse_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "打开json文件", "./", "Json文件 (*.json)");
    if(path.isEmpty()) return;

    // 将json数据保存到文件
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "打开文件失败！";
        return;
    }

    // 读取json数据
    QByteArray arr = file.readAll();
    file.close();
    if(arr.isEmpty()) return;

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(arr, &err);
    if(doc.isEmpty())   // 如果加载json数据失败则打印失败信息
    {
        qWarning() << err.errorString();
    }

    ui->textEdit->append("<------------- 解析json ------------->");
    if(doc.isArray())        // 如果json数据为数组类型
    {
        jsonParse(QJsonValue(doc.array()));
    }
    else if(doc.isObject())  // 如果Json数据为对象类型
    {
        jsonParse(QJsonValue(doc.object()));
    }
    else
    {
    }
}

/**
 * @brief       json解析，假定不知道json数据的结构，那么通过递归来解析就很不错
 * @param value
 */
void Widget::jsonParse(const QJsonValue &value)
{
    // 这里可以使用type() 函数，也可以使用isNull()、isBool()等函数
    switch (value.type())
    {
    case QJsonValue::Null:
    {
        break;
    }
    case QJsonValue::Bool:
    {
        ui->textEdit->append(QString("bool类型数据：%1").arg(value.toBool()));
        break;
    }
    case QJsonValue::Double:
    {
        ui->textEdit->append(QString("double类型数据：%1").arg(value.toDouble()));
        break;
    }
    case QJsonValue::String:
    {
        ui->textEdit->append(QString("字符串类型数据：%1").arg(value.toString()));
        break;
    }
    case QJsonValue::Array:
    {
        ui->textEdit->append("数组：------------- start");
        QJsonArray arr = value.toArray();
        for(auto v : arr)   // 遍历数组
        {
            jsonParse(v);   // 使用递归的方式解析数组内容
        }
        ui->textEdit->append("数组：------------- end");
        break;
    }
    case QJsonValue::Object:
    {
        ui->textEdit->append("对象：------------- start");
        QJsonObject obj = value.toObject();
        QStringList keys = obj.keys();        // 获取所有的键值
        for(auto key : keys)
        {
            ui->textEdit->append(QString("键值：%1").arg(key));
            jsonParse(obj[key]);           // 通过键值获取数据，并调用解析函数递归解析
//            jsonParse(obj.take(key));    // 使用toke和使用[]效果一样
        }
        ui->textEdit->append("对象：------------- end");
        break;
    }
    case QJsonValue::Undefined:
    {
        break;
    }
    }
}
