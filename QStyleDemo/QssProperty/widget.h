#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)
    Q_PROPERTY(QColor BgColor READ isBgColor WRITE setBgColor)
    Q_PROPERTY(AgeEnum age READ age WRITE setAge)               // 想要通过Q_ENUM注册的枚举修改属性值，属性的类型就需要时【枚举的类型】，而不能是其它类型，例如int

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    enum AgeEnum {
        age1 = 10,
        age2 = 20,
        age3 = 30
    };
    Q_ENUM(AgeEnum)  // 向元对象系统注册枚举类型(可以使用Q_ENUM或者Q_ENUMS，不过后者已经过时)

    bool isChecked() const;
    void setChecked(bool value);
    QColor isBgColor() const;
    void setBgColor(QColor color);
    AgeEnum age() const;
    void setAge(AgeEnum value);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    void initStyle();

private:
    Ui::Widget *ui;

    bool m_checked = false;
    QColor m_bgColor = QColor(255, 255, 255);
    AgeEnum m_age;
};
#endif // WIDGET_H
