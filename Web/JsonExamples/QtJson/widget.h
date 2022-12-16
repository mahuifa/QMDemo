#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void jsonParse(const QJsonValue& value);

private slots:
    void on_but_create_clicked();
    
    void on_but_parse_clicked();
    
    void on_but_save_clicked();

private:
    Ui::Widget *ui;

    QJsonDocument m_doc;    // 创建一个用于读写json的对象
};
#endif // WIDGET_H
