#include "playimage.h"


#if USE_WINDOW
PlayImage::PlayImage(QOpenGLWindow::UpdateBehavior updateBehavior, QWindow *parent):QOpenGLWindow(updateBehavior, parent)
{
}
#else
PlayImage::PlayImage(QWidget *parent, Qt::WindowFlags f): QOpenGLWidget(parent, f)
{

}
#endif


PlayImage::~PlayImage()
{
    if(!isValid()) return;        // 如果控件和OpenGL资源（如上下文）已成功初始化，则返回true。
    this->makeCurrent(); // 通过将相应的上下文设置为当前上下文并在该上下文中绑定帧缓冲区对象，为呈现此小部件的OpenGL内容做准备。
    // 释放纹理
    if(m_texture)
    {
        m_texture->destroy();
        delete m_texture;
    }
    this->doneCurrent();    // 释放上下文
    // 释放
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

/**
 * @brief        传入Qimage图片显示
 * @param image
 */
void PlayImage::updateImage(const QImage& image)
{
    if(image.isNull()) return;

    m_size = image.size();
    if(!m_texture)
    {
        m_texture = new QOpenGLTexture(image.mirrored());
        resizeGL(this->width(), this->height());
    }
    else
    {
        m_texture->destroy();
        m_texture->setData(image.mirrored());
    }
    this->update();
}
// 三个顶点坐标XYZ，VAO、VBO数据播放，范围时[-1 ~ 1]直接
static GLfloat vertices[] = {  // 前三列点坐标，后两列为纹理坐标
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,      // 右上角
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,      // 右下
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,      // 左下
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f      // 左上
};
static GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};
void PlayImage::initializeGL()
{
    initializeOpenGLFunctions();

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.fsh");
    m_program->link();

    // 返回属性名称在此着色器程序的参数列表中的位置。如果名称不是此着色器程序的有效属性，则返回-1。
    GLuint posAttr = GLuint(m_program->attributeLocation("aPos"));
    GLuint texCord = GLuint(m_program->attributeLocation("aTexCord"));

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGenBuffers(1, &EBO);    // 创建一个EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    // 为当前绑定到的缓冲区对象创建一个新的数据存储target。任何预先存在的数据存储都将被删除。
    glBufferData(GL_ARRAY_BUFFER,        // 为VBO缓冲绑定顶点数据
                       sizeof (vertices),      // 数组字节大小
                       vertices,               // 需要绑定的数组
                       GL_STATIC_DRAW);        // 指定数据存储的预期使用模式,GL_STATIC_DRAW： 数据几乎不会改变
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  // 将顶点索引数组传入EBO缓存
    // 设置顶点坐标数据
    glVertexAttribPointer(posAttr,                     // 指定要修改的通用顶点属性的索引
                          3,                     // 指定每个通用顶点属性的组件数（如vec3：3，vec4：4）
                          GL_FLOAT,              // 指定数组中每个组件的数据类型(数组中一行有几个数)
                          GL_FALSE,              // 指定在访问定点数据值时是否应规范化 ( GL_TRUE) 或直接转换为定点值 ( GL_FALSE)，如果vertices里面单个数超过-1或者1可以选择GL_TRUE
                          5 * sizeof(GLfloat),   // 指定连续通用顶点属性之间的字节偏移量。
                          nullptr);              // 指定当前绑定到目标的缓冲区的数据存储中数组中第一个通用顶点属性的第一个组件的偏移量。初始值为0 (一个数组从第几个字节开始读)
    // 启用通用顶点属性数组
    glEnableVertexAttribArray(posAttr);                // 属性索引是从调用glGetAttribLocation接收的，或者传递给glBindAttribLocation。

    // 设置纹理坐标数据
    glVertexAttribPointer(texCord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<const GLvoid *>(3 * sizeof (GLfloat)));              // 指定当前绑定到目标的缓冲区的数据存储中数组中第一个通用顶点属性的第一个组件的偏移量。初始值为0 (一个数组从第几个字节开始读)
    // 启用通用顶点属性数组
    glEnableVertexAttribArray(texCord);                // 属性索引是从调用glGetAttribLocation接收的，或者传递给glBindAttribLocation。

    // 释放
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);                        // 设置为零以破坏现有的顶点数组对象绑定

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);        // 指定颜色缓冲区的清除值(背景色)
}

void PlayImage::resizeGL(int w, int h)
{
    if(m_size.width()  < 0 || m_size.height() < 0) return;

    // 计算需要显示图片的窗口大小，用于实现长宽等比自适应显示
    if((double(w) / h) < (double(m_size.width()) / m_size.height()))
    {
        m_zoomSize.setWidth(w);
        m_zoomSize.setHeight(((double(w) / m_size.width()) * m_size.height()));   // 这里不使用QRect，使用QRect第一次设置时有误差bug
    }
    else
    {
        m_zoomSize.setHeight(h);
        m_zoomSize.setWidth((double(h) / m_size.height()) * m_size.width());
    }
    m_pos.setX(double(w - m_zoomSize.width()) / 2);
    m_pos.setY(double(h - m_zoomSize.height()) / 2);
    this->update(QRect(0, 0, w, h));
}

void PlayImage::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);     // 将窗口的位平面区域（背景）设置为先前由glClearColor、glClearDepth和选择的值
    glViewport(m_pos.x(), m_pos.y(), m_zoomSize.width(), m_zoomSize.height());  // 设置视图大小实现图片自适应

    m_program->bind();               // 绑定着色器
    if(m_texture)
    {
        m_texture->bind();
    }

    glBindVertexArray(VAO);           // 绑定VAO

    glDrawElements(GL_TRIANGLES,      // 绘制的图元类型
                   6,                 // 指定要渲染的元素数(点数)
                   GL_UNSIGNED_INT,   // 指定索引中值的类型(indices)
                   nullptr);          // 指定当前绑定到GL_ELEMENT_array_buffer目标的缓冲区的数据存储中数组中第一个索引的偏移量。
    glBindVertexArray(0);
    if(m_texture)
    {
        m_texture->release();
    }
    m_program->release();
}
