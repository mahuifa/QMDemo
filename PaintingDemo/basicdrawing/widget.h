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
    void connectSlots();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;


private slots:
    void setPen();
    void on_horizontalSlider_origin_valueChanged(int value);

    void on_verticalSlider_origin_valueChanged(int value);

    void on_checkBox_clicked(bool checked);

    void on_but_color_clicked();

    void on_com_brushStyle_activated(int index);

    void on_com_shape_activated(int index);

    void on_dial_rotate_valueChanged(int value);

private:
    Ui::Widget *ui;
    QColor m_color;
    QBrush m_penBrush;
};
#endif // WIDGET_H
