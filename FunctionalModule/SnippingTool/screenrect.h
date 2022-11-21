/******************************************************************************
 * @文件名     screenrect.h
 * @功能       矩形截图选框窗口
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/11/18
 * @备注
 *****************************************************************************/
#ifndef SCREENRECT_H
#define SCREENRECT_H

#include <QWidget>

class ScreenRect : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenRect(QWidget *parent = nullptr);

signals:
    void selectRect(QRect rect);

protected:
    void mousePressEvent(QMouseEvent *event)   override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)    override;
    void paintEvent(QPaintEvent *event)        override;

private:
    QRect m_rect;         // 选择的矩形
    QPainterPath m_path;
};

#endif // SCREENRECT_H
