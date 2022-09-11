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

class SliderVideo : public QSlider
{
    Q_OBJECT
public:
    explicit SliderVideo(QWidget *parent = nullptr);

signals:

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
};

#endif // SLIDERVIDEO_H
