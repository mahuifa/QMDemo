/******************************************************************************
 * @文件名     sidebar.h
 * @功能       自定义侧边栏，用于显示播放列表
 *            1、支持动画打开关闭功能；
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/03/27
 * @备注
 *****************************************************************************/
#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>


namespace Ui {
class Sidebar;
}

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);
    ~Sidebar();

protected:
    void on_finished();

private:
    Ui::Sidebar *ui;

};

#endif // SIDEBAR_H
