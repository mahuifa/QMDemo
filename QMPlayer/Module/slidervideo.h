/******************************************************************************
 * @文件名     slidervideo.h
 * @功能       自定义视频播放进度条
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/03
 * @备注
 *****************************************************************************/
#ifndef SLIDERVIDEO_H
#define SLIDERVIDEO_H

#include <QSlider>

class QPropertyAnimation;
class SliderVideo : public QSlider
{
    Q_OBJECT
public:
    explicit SliderVideo(QWidget *parent = nullptr);

    void show();
    void hide();

signals:

protected:
    void on_finished();
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPropertyAnimation* m_paShow = nullptr;          // 动画对象，负责打开关闭侧边栏窗口动画
};

#endif // SLIDERVIDEO_H
