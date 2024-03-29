#include "widget.h"
#include "renderarea.h"
#include "QApplication"
#include <qcombobox.h>
#include <qdebug.h>
#include <qgridlayout.h>

Widget::Widget(QWidget *parent): QWidget(parent)
{
    this->resize(800, 700);
    qApp->setStyleSheet("*{font: 9pt '宋体';}");
    this->setWindowTitle(QString("QPainter偏移原点、旋转、缩放功能Demo - V%1").arg(APP_VERSION));
    initUI();
    setupShapes();         // 设置形状
}

Widget::~Widget()
{
}

void Widget::initUI()
{
    m_originalRenderArea = new RenderArea(this);
    m_comShape = new QComboBox(this);
    m_comShape->addItem("卡车图案");
    m_comShape->addItem("时钟图案");
    m_comShape->addItem("房子图案");
    m_comShape->addItem("文字图案");
    // 这里可以用activated也可以用currentIndexChanged，由于这两个信号都有重载，所以需要加上QOverload
    connect(m_comShape, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::shapeSelected);

    // 添加布局
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(m_originalRenderArea, 0, 0);
    layout->addWidget(m_comShape, 1, 0);

    for(int i = 0; i < NumTransformendAreas; i++)
    {
        m_transformedRenderAreas[i] = new RenderArea(this);
        m_comOperation[i] = new QComboBox(this);
        m_comOperation[i]->addItem("默认状态");
        m_comOperation[i]->addItem("坐标原点偏移（50,50）");
        m_comOperation[i]->addItem("旋转60度");
        m_comOperation[i]->addItem("缩小75%");
        connect(m_comOperation[i], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::operationChanged);

        // 添加布局
        layout->addWidget(m_transformedRenderAreas[i], 0, i + 1);
        layout->addWidget(m_comOperation[i], 1, i + 1);
    }
}

void Widget::setupShapes()
{
    // 绘制卡车图案的路径
    QPainterPath truck;

    truck.setFillRule(Qt::WindingFill);   // 设置填充规则
    truck.moveTo(0.0, 87.0);              // 将绘制起点移动到给定点，隐式启动新的子路径并关闭前一个子路径。
    truck.lineTo(0.0, 60.0);              // 添加从当前位置到给定端点的直线。绘制线后，当前位置将更新为线的端点。
    truck.lineTo(10.0, 60.0);
    truck.lineTo(35.0, 35.0);
    truck.lineTo(100.0, 35.0);
    truck.lineTo(100.0, 87.0);
    truck.lineTo(0.0, 87.0);

    truck.moveTo(17.0, 60.0);
    truck.lineTo(55.0, 60.0);
    truck.lineTo(55.0, 40.0);
    truck.lineTo(37.0, 40.0);
    truck.lineTo(17.0, 60.0);
    truck.addEllipse(17.0, 75.0, 25.0, 25.0);  // 添加一个椭圆，椭圆由一条【顺时针曲线】组成，起点和终点为零度（3点钟位置）
    truck.addEllipse(63.0, 75.0, 25.0, 25.0);

    // 绘制时钟的路径
    QPainterPath clock;
    clock.addEllipse(-50.0, -50.0, 100.0, 100.0);
    clock.addEllipse(-48.0, -48.0, 96.0, 96.0);
    clock.moveTo(0.0, 0.0);
    clock.lineTo(-2.0, -2.0);
    clock.lineTo(0.0, -42.0);
    clock.lineTo(2.0, -2.0);
    clock.lineTo(0.0, 0.0);
    clock.moveTo(0.0, 0.0);
    clock.lineTo(2.732, -0.732);
    clock.lineTo(24.495, 14.142);
    clock.lineTo(0.732, 2.732);
    clock.lineTo(0.0, 0.0);

    // 绘制房子路径
    QPainterPath house;
    house.moveTo(-45.0, -20.0);
    house.lineTo(0.0, -45.0);
    house.lineTo(45.0, -20.0);
    house.lineTo(45.0, 45.0);
    house.lineTo(-45.0, 45.0);
    house.lineTo(-45.0, -20.0);
    house.addRect(15.0, 5.0, 20.0, 35.0);     // 添加一个矩形，矩形被添加为一组顺时针方向的直线。添加矩形后，绘制路径的当前位置位于矩形的【左上角】
    house.addRect(-35.0, -15.0, 25.0, 25.0);

    // 绘制文本路径
    QPainterPath text;
    QFont font;
    font.setPixelSize(50);   // 设置文字大小
    QRect fontRect = QFontMetrics(font).boundingRect("1a你好");  // 计算输入字符串在指定的font字体时所占的矩形大小
    text.addText(-QPointF(fontRect.center()), font, "1a你好");   // 添加需要绘制的字体

    // 将创建的绘画路径添加进列表
    m_shapes.append(truck);
    m_shapes.append(clock);
    m_shapes.append(house);
    m_shapes.append(text);

    shapeSelected(0);        // 显示第一个图案
}

/**
 * @brief       当下拉框选择图案后通过信号修改显示的图案路径
 * @param index
 */
void Widget::shapeSelected(int index)
{
    QPainterPath shape = m_shapes.at(index);
    m_originalRenderArea->setShape(shape);

    for(int i = 0; i < NumTransformendAreas; i++)
    {
        m_transformedRenderAreas[i]->setShape(shape);
    }
}

/**
 * @brief 设置绘制图案的原点偏移、旋转、缩放
 */
void Widget::operationChanged(int index)
{
    Q_UNUSED(index)

    QList<Operation> operations;
    for(int i = 0; i < NumTransformendAreas; i++)
    {
        operations.append(Operation(m_comOperation[i]->currentIndex()));
        m_transformedRenderAreas[i]->setOperations(operations);   // 第一个绘图operations长度为1，第二个operations为2，第三个operations长度为3
    }
}

