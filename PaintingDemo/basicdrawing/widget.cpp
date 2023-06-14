#include "widget.h"
#include "ui_widget.h"
#include "renderarea.h"
#include <QColorDialog>
#include <QDebug>
#include <QMetaEnum>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("QPainter基本绘图Demo - V%1").arg(APP_VERSION));
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");

    init();
    connectSlots();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    // 获取画图形状Shape枚举名称和值，写入下拉框
    QMetaEnum metaEnum = QMetaEnum::fromType<RenderArea::Shape>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_shape->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 画笔样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 画笔笔帽样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenCapStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penCapStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 线段转角处连接样式枚举
    metaEnum = QMetaEnum::fromType<Qt::PenJoinStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_penJoinStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    // 笔刷样式枚举
    metaEnum = QMetaEnum::fromType<Qt::BrushStyle>();
    for(int i=0; i < metaEnum.keyCount(); ++i)
    {
        ui->com_brushStyle->addItem(metaEnum.key(i), metaEnum.value(i));
    }

    ui->widget->installEventFilter(this);
    ui->com_shape->setCurrentIndex(0);
}

void Widget::connectSlots()
{
    // activated()只有用户操作时才触发，currentIndexChanged()时用户操作或者程序更改时触发，所以这里需要使用currentIndexChanged()
    connect(ui->com_penStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::setPen);
    connect(ui->com_penCapStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::setPen);
    connect(ui->com_penJoinStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::setPen);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Widget::setPen);

    // 由于init()在connectSlots()前，所以这几行代码需要触发信号就只能放在connectSlots()后
    ui->com_penStyle->setCurrentIndex(1);
    ui->com_penJoinStyle->setCurrentIndex(0);
}

/**
 * @brief 设置画笔
 */
void Widget::setPen()
{
    QPen pen;
    pen.setStyle(Qt::PenStyle(ui->com_penStyle->currentData().toInt()));   // 设置画笔样式
    Qt::PenJoinStyle joinStyle = Qt::PenJoinStyle(ui->com_penJoinStyle->currentData().toInt());
    pen.setJoinStyle(joinStyle);
    if(Qt::MiterJoin == joinStyle)   // 笔样式设置为 Qt::MiterJoin 时才有效
    {
        pen.setMiterLimit(0.2);   // 将此笔的斜接限制设置为0.2，笔的宽度的倍数。
    }
    pen.setWidth(ui->spinBox->value());     // 设置画笔线宽
    pen.setColor(m_color);                  // 设置画笔颜色
//    pen.setBrush()
    pen.setCapStyle(Qt::PenCapStyle(ui->com_penCapStyle->currentData().toInt()));  // 设置画笔笔帽样式
    ui->widget->setPen(pen);
}

/**
 * @brief            事件过滤器
 * @param watched
 * @param event
 * @return
 */
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget && event->type() == QEvent::Resize)
    {
        ui->horizontalSlider_origin->setMaximum(ui->widget->width());
        ui->verticalSlider_origin->setMaximum(ui->widget->height());
        ui->horizontalSlider_origin->setValue(0);
        ui->verticalSlider_origin->setValue(ui->widget->height());
    }

    return  QWidget::eventFilter(watched, event);
}

/**
 * @brief         水平偏移原点
 * @param value
 */
void Widget::on_horizontalSlider_origin_valueChanged(int value)
{
    ui->widget->setTranslate(value,  ui->widget->height() - ui->verticalSlider_origin->value());
}

/**
 * @brief        垂直偏移原点
 * @param value
 */
void Widget::on_verticalSlider_origin_valueChanged(int value)
{
    ui->widget->setTranslate(ui->horizontalSlider_origin->value(), ui->widget->height() - value);
}


/**
 * @brief           设置抗锯齿
 * @param checked
 */
void Widget::on_checkBox_clicked(bool checked)
{
    ui->widget->setAntialiased(checked);
}

/**
 * @brief 设置画笔颜色
 */
void Widget::on_but_color_clicked()
{
    m_color = QColorDialog::getColor(m_color, this);
    setPen();
}

/**
 * @brief        设置笔刷
 * @param index
 */
void Widget::on_com_brushStyle_activated(int index)
{
    Q_UNUSED(index)
    Qt::BrushStyle style = Qt::BrushStyle(ui->com_brushStyle->currentData().toInt());

    switch (style)
    {
    case Qt::LinearGradientPattern:    // 添加线性渐变图案
    {
        QLinearGradient linearGradient(0, 0, 200, 200);    // 从QPoint(0, 0)到QPoint(200, 200)的线性渐变色
        linearGradient.setColorAt(0.0, Qt::white);
        linearGradient.setColorAt(0.5, Qt::green);
        linearGradient.setColorAt(1.0, Qt::black);
        ui->widget->setBrush(linearGradient);
        break;
    }
    case Qt::RadialGradientPattern:   // 添加径向渐变图案
    {
        QRadialGradient radialGradient(QPointF(150, 150), 150, QPointF(100, 100));  // 中心点QPointF(150, 150)，圆形半径150，焦点QPointF(100, 100)
        radialGradient.setColorAt(0.0, Qt::white);
        radialGradient.setColorAt(0.3, Qt::green);
        radialGradient.setColorAt(1.0, Qt::black);
        ui->widget->setBrush(radialGradient);
        break;
    }
    case Qt::ConicalGradientPattern:   // 添加锥形渐变图案
    {
        QConicalGradient conicalGradient(150, 150, 90);    // 中心点QPointF(150, 150)，角度90（0度为3点钟方向）
        conicalGradient.setColorAt(0.0, Qt::white);
        conicalGradient.setColorAt(0.5, Qt::green);
        conicalGradient.setColorAt(1.0, Qt::black);
        ui->widget->setBrush(conicalGradient);
        break;
    }
    case Qt::TexturePattern:           // 添加图片纹理笔刷
    {
        ui->widget->setBrush(QBrush(QPixmap("C:/Users/mhf/Pictures/grid.PNG")));
        break;
    }
    default:
    {
        QBrush brush;
        brush.setStyle(style);
        brush.setColor(Qt::red);
        ui->widget->setBrush(brush);
        break;
    }
    }
}

/**
 * @brief        设置绘制图案类型
 * @param index
 */
void Widget::on_com_shape_activated(int index)
{
    Q_UNUSED(index)
    ui->widget->setShape(RenderArea::Shape(ui->com_shape->currentData().toInt()));
}

/**
 * @brief         旋转
 * @param value
 */
void Widget::on_dial_rotate_valueChanged(int value)
{
    ui->widget->setRotate((value + 90) % 360);     // 由于QDial默认0为竖直向下方向，QPainter默认0度为向右，所以需要+90度
}
