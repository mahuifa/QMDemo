#include "openglwidget.h"
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setSingleBuffer(true);
    connect(&m_timer, &QTimer::timeout, this, &OpenGLWidget::on_timer);
    angle = 0;
}

void OpenGLWidget::setSingleBuffer(bool flag)
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);       // 设置所需的可渲染类型。
    format.setSwapInterval(0);                              // 设置首选交换间隔
    // 设置交换行为
    if(flag)
    {
        format.setSwapBehavior(QSurfaceFormat::SingleBuffer);   // 使用单缓冲，OpenGL直接渲染到屏幕上，速度很快，但是GPU消耗很大，并且可能会闪烁
    }
    else
    {
        format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);   // 使用双缓冲（默认），在后缓冲渲染，再复制到前缓冲进行显示，GPU消耗低于单缓冲，不会闪烁，但是速度慢
    }
    QSurfaceFormat::setDefaultFormat(format);
}

void OpenGLWidget::start()
{
    m_timer.start(1);
}

void OpenGLWidget::stop()
{
    m_timer.stop();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();               // 在QOpenGLFunctions中
    glClearColor(0.0, 0.0, 0.0, 0.0);          // 提前设置背景色，需要使用glClear
    glClearDepth(1.0);                         // 设置清除深度，1.0是最大深度([0.0,1.0])
}

void OpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // 清除缓冲区并使用glClearColor的颜色设置背景, GL_COLOR_BUFFER_BIT:指示当前为颜色写入启用的缓冲区;GL_DEPTH_BUFFER_BIT：指示深度缓冲区
    glLoadIdentity();                                     // 重置当前指定的矩阵为单位矩阵
    glRotatef(angle,0.0,1.0,0.0);                         // 旋转，四个参数，旋转角度，旋转轴 x y z
    glBegin(GL_TRIANGLES);                                // 开始绘制三角形，用于定义一个或者多个图元的顶点的开始。此函数通常与glEnd函数联用

    glColor3f(1.0f, 0.0, 0.0);                               // 设置当前颜色，范围[0,1]
    glVertex3f(0.0, 0.8f, 0.0);                              // 设置顶点坐标

    glColor3f(0.0, 0.0, 1.0f);
    glVertex3f(0.5f, 0.0, 0.0);

    glColor3f(0.0, 1.0f, 0.0);
    glVertex3f(-0.5f, 0.0, 0.0);

    glEnd();
}

void OpenGLWidget::on_timer()
{
    angle++;
    this->update();
}
