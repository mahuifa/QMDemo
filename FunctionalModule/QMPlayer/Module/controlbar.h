/******************************************************************************
 * @文件名     controlbar.h
 * @功能       自定义播放控制栏
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/03/26
 * @备注
 *****************************************************************************/
#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QWidget>

class QPropertyAnimation;

namespace Ui {
class ControlBar;
}

class ControlBar : public QWidget
{
    Q_OBJECT

public:
    explicit ControlBar(QWidget *parent = nullptr);
    ~ControlBar();

    void show();
    void hide();

protected:
    void on_finished();

private slots:
    void on_but_play_clicked();

    void on_but_volume_clicked();

private:
    Ui::ControlBar *ui;

    QPropertyAnimation* m_paShow = nullptr;          // 动画对象，负责打开关闭侧边栏窗口动画
    bool m_play = false;                             // 播放状态
    bool m_volume = true;                            // 音量状态
};

#endif // CONTROLBAR_H
