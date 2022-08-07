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

namespace Ui {
class ControlBar;
}

class ControlBar : public QWidget
{
    Q_OBJECT

public:
    explicit ControlBar(QWidget *parent = nullptr);
    ~ControlBar();

private:
    Ui::ControlBar *ui;
};

#endif // CONTROLBAR_H
