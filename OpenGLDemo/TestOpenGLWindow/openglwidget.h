/******************************************************************************
 * @文件名     openglwidget.h
 * @功能       通过继承QOpenGLWidget类绘制一个三角形
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/09/30
 * @备注
 *****************************************************************************/
#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <qopenglwidget.h>
//#include <QOpenGLFunctions>                            // 网上很多示例使用的是这个类，这是Qt5.0添加的，编译时需要在pro文件中链接opengl库LIBS +=-lopengl32 -lglu32
#include <QElapsedTimer>
#include <QOpenGLFunctions_4_5_Compatibility>            // 由于使用到的glBegin等函数属于opengl1.0的API，在新版本中已经被删除了，所以这里需要使用Compatibility
#include <qtimer.h>

class QOpenGLShaderProgram;

#define OUT_FUN_WIDGET 0           // 1：使用已经过时的API绘制三角形（简单），0：使用新的API绘制三角形（较为复杂）

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Compatibility
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

    void rotate();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void setSingleBuffer(bool flag);

private:
    int m_angle;                                // 旋转角度
#if OUT_FUN_WIDGET == 0
    QOpenGLShaderProgram* m_program = nullptr;  // 使用QOpenGLShaderProgram链接和使用OpenGL着色器程序
    int  m_posAttr;                             // 顶点坐标属性位置
    int  m_colAttr;                             // 顶点颜色属性位置
    int  m_matrixUniform;                       // 变换矩阵属性位置
#endif
    QElapsedTimer m_timer;                             // 刷新定时器
};

#endif // OPENGLWIDGET_H
