#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QToolButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_sourceButton_clicked();

    void on_destinationButton_clicked();

    void on_operatorComboBox_currentIndexChanged(int index);

private:
    void addOp(QPainter::CompositionMode mode, const QString &name);
    void loadImage(const QString &fileName, QImage& image, QToolButton *button);
    QPointF imagePos(const QImage& image) const;
    void recalculateResult();

private:
    Ui::Widget *ui;
    QImage sourceImage;
    QImage destinationImage;
    QImage resultImage;               // 合成后的图片
};
#endif // WIDGET_H
