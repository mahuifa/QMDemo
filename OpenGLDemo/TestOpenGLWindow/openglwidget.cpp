#include "openglwidget.h"
#include <QDebug>
#include <qopenglshaderprogram.h>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setSingleBuffer(true);
    m_angle = 0;
}

void OpenGLWidget::setSingleBuffer(bool flag)
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);       // 设置所需的可渲染类型。
//    format.setSwapInterval(0);                              // 设置首选交换间隔，如果设置成0则刷新速度会很快，但是GPU消耗很大
    // 设置交换行为
    if(flag)
    {
        format.setSwapBehavior(QSurfaceFormat::SingleBuffer);   // 使用单缓冲，OpenGL直接渲染到屏幕上，并且可能会闪烁
    }
    else
    {
        format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);   // 使用双缓冲（默认），在后缓冲渲染，再复制到前缓冲进行显示，GPU消耗低于单缓冲，不会闪烁，速度慢
    }
    QSurfaceFormat::setDefaultFormat(format);
}


void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();               // 在QOpenGLFunctions中
    glClearColor(0.0, 0.0, 0.0, 0.0);          // 提前设置背景色，需要使用glClear
    glClearDepth(1.0);                         // 设置清除深度，1.0是最大深度([0.0,1.0])
#if OUT_FUN_WIDGET
#else
    // 加载shader脚本(顶点和片元)
    m_program = new QOpenGLShaderProgram(this);
    // 直接通过文件加载Shader脚本，如果加载返回false则加载失败，如果提示ERROR: error(#60) Unknown char: "???"，
    // 可以使用Notepad++打开vsh/fsh文件看一下文件编码是不是utf-8，如果不是或者时utf-8 DOM就不行
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertexShader.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragmentShader.fsh");
    m_program->link();   // 编译链接着色器

    // 返回属性名称在此着色器程序的参数列表中的位置。如果名称不是此着色器程序的有效属性，则返回-1。
    m_posAttr = m_program->attributeLocation("qt_Vertex");
    m_colAttr = m_program->attributeLocation("qt_MultiTexCoord0");
    // 返回统一变量名称在此着色器程序的参数列表中的位置。如果名称不是此着色器程序的有效统一变量，则返回-1。
    m_matrixUniform = m_program->uniformLocation("qt_ModelViewProjectionMatrix");
#endif
}

void OpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
    update();
}

#if OUT_FUN_WIDGET == 0
// 需要绘制的顶点坐标（x, y），范围[-1, 1]
const GLfloat vertices[] = {
    0.0f,  0.7f,
    -0.5f, -0.5f,
    0.5f,  -0.5f
};
// 需要绘制的顶点颜色(r, g, b)，范围[0, 1]
const GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};
#endif

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // 清除缓冲区并使用glClearColor的颜色设置背景, GL_COLOR_BUFFER_BIT:指示当前为颜色写入启用的缓冲区;GL_DEPTH_BUFFER_BIT：指示深度缓冲区

#if OUT_FUN_WIDGET
    glLoadIdentity();                                     // 重置当前指定的矩阵为单位矩阵
    glRotatef(m_angle,0.0,1.0,0.0);                       // 旋转，四个参数，旋转角度，旋转轴 x y z

    glBegin(GL_TRIANGLES);                                // 开始绘制三角形，用于定义一个或者多个图元的顶点的开始。此函数通常与glEnd函数联用

    glColor3f(1.0f, 0.0, 0.0);                            // 设置当前颜色，范围[0,1]
    glVertex3f(0.0, 0.8f, 0.0);                           // 设置顶点坐标，范围[-1, 1]

    glColor3f(0.0, 0.0, 1.0f);
    glVertex3f(0.8f, -0.8f, 0.0);

    glColor3f(0.0, 1.0f, 0.0);
    glVertex3f(-0.8f, -0.8f, 0.0);

    glEnd();
#else
    const qreal retinaScale = devicePixelRatio();   // 返回窗口的物理像素与设备无关像素之间的比率。此值取决于窗口所在的屏幕，并可能在移动窗口时更改。
    glViewport(0, 0, GLsizei(this->width() * retinaScale), GLsizei(this->height() * retinaScale));  // 设置视图大小
    m_program->bind();                              // 将此着色器程序绑定到活动的QOpenGLContext，并使其成为当前着色器程序

    // 表示3D空间中的4x4变换矩阵
    QMatrix4x4 matrix;
    matrix.perspective(40.0f,                               // 透视投影, verticalAngle: 垂直角度(也就是裁剪窗口的高度),值越大,那么物体越小
                       this->width() / this->height(),      // 纵横比(由视口的宽除以高所得)
                       0.1f,                                // 设置近平面的距离(一般设置为0.1)
                       100.0f);                             // 设置远平面的距离(一般设置为100)
    matrix.translate(0, 0, -2);                             // 坐标沿Z轴偏移
    matrix.rotate(m_angle, 0, 1, 0);                        // 绕着Y轴旋转

    m_program->setUniformValue(m_matrixUniform, matrix);   // 将当前上下文中位置处的统一变量设置为value。

    // 设置顶点坐标
    glVertexAttribPointer(GLuint(m_posAttr),    // 指定属性的索引
                          2,                    // 指定每个通用顶点属性的组件数。必须是 1、2、3、4
                          GL_FLOAT,             // 指定数组中的数据类型
                          GL_FALSE,             // 指定在访问定点数据值时是否应归一化(GL_TRUE)
                          0,                    // 指定连续通用顶点属性之间的字节偏移量。如果 stride 为 0，则通用顶点属性被理解为紧密封装在数组中。初始值为0
                          vertices);            // 顶点数组指针
    // 设置顶点颜色
    glVertexAttribPointer(GLuint(m_colAttr), 3, GL_FLOAT, GL_FALSE, 0, colors);

    // 启用顶点数组
    glEnableVertexAttribArray(GLuint(m_posAttr));    // 属性索引是从调用glGetAttribLocation接收的，或者传递给glBindAttribLocation。
    glEnableVertexAttribArray(GLuint(m_colAttr));

    // 提供绘制功能，从数组数据中提取数据渲染基本图元
    glDrawArrays(GL_TRIANGLES,     // 图元类型
                 0,                // 从数组中哪一位开始
                 3);               // 数组中顶点的数量

    // 禁用顶点属性数组
    glDisableVertexAttribArray(GLuint(m_posAttr));
    glDisableVertexAttribArray(GLuint(m_colAttr));

    m_program->release();         // 从当前QOpenGLContext中释放活动着色器程序。
#endif
}

void OpenGLWidget::rotate()
{
    m_angle++;
    this->update();
    if(m_angle % 360 == 0)
    {
        qDebug() << "QOpenGLWidget：" << m_timer.elapsed();
        m_timer.start();
    }
}
