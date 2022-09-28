#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <qopenglwidget.h>
//#include <QOpenGLFunctions>                            // 网上很多示例使用的是整个类，这是Qt5.0添加的，编译时需要在pro文件中链接opengl库LIBS +=-lopengl32 -lglu32
#include <QOpenGLFunctions_4_5_Compatibility>            // 由于使用到的glBegin等函数属于opengl1.0的API，在新版本中已经被删除了，所以这里需要使用Compatibility
#include <qtimer.h>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Compatibility
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

    void start();
    void stop();

signals:

public slots:

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void on_timer();
    void setSingleBuffer(bool flag);

private:
    GLfloat angle;
    QTimer m_timer;
};

#endif // OPENGLWIDGET_H
