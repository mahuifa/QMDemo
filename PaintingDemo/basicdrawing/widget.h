#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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
    void init();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;


private slots:
    void on_horizontalSlider_origin_valueChanged(int value);

    void on_verticalSlider_origin_valueChanged(int value);

    void on_horizontalSlider_rotate_valueChanged(int value);

    void on_checkBox_clicked(bool checked);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
