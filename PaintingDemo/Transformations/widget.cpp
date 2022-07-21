#include "widget.h"
#include "renderarea.h"

#include <qcombobox.h>
#include <qgridlayout.h>

Widget::Widget(QWidget *parent): QWidget(parent)
{
    initUI();
}

Widget::~Widget()
{
}

void Widget::initUI()
{
    m_originalRenderArea = new RenderArea(this);
    m_comShape = new QComboBox(this);
    m_comShape->addItem("时钟图案");
    m_comShape->addItem("房子图案");
    m_comShape->addItem("文字图案");
    m_comShape->addItem("卡车图案");

    // 添加布局
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(m_originalRenderArea, 0, 0);
    layout->addWidget(m_comShape, 1, 0);

    for(int i = 0; i < NumTransformendAreas; i++)
    {
        m_transformedRenderAreas[i] = new RenderArea(this);
        m_comOperation[i] = new QComboBox(this);
        m_comOperation[i]->addItem("默认状态");
        m_comOperation[i]->addItem("旋转60度");
        m_comOperation[i]->addItem("缩小75%");
        m_comOperation[i]->addItem("坐标原点偏移（50,50）");

        // 添加布局
        layout->addWidget(m_transformedRenderAreas[i], 0, i + 1);
        layout->addWidget(m_comOperation[i], 1, i + 1);
    }
}

