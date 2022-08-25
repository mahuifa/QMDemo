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

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void reset();

private slots:

    void on_but_reset_clicked();

    void on_but_transform_clicked();

    void on_but_original_clicked();

    void on_horizontalSlider_origin_valueChanged(int value);

    void on_verticalSlider_origin_valueChanged(int value);

    void on_dial_valueChanged(int value);

    void on_horizontalSlider_scale_valueChanged(int value);

private:
    Ui::Widget *ui;

    QTransform m_transform;
};
#endif // WIDGET_H
