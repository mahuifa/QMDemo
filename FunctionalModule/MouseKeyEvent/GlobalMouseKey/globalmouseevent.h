/******************************************************************************
 * @文件名     mouseevent.h
 * @功能       全局鼠标事件监听类
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/12/07
 * @备注
 *****************************************************************************/
#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QObject>

/**
 *  全局鼠标事件单例信号类
 */
class GlobalMouseEvent : public QObject
{
    Q_OBJECT
public:
    static GlobalMouseEvent* getInstance()
    {
        static GlobalMouseEvent mouseEvent;
        return &mouseEvent;
    }

    static bool installMouseEvent();      // 安装全局鼠标事件监听器
    static bool removeMouseEvent();       // 卸载全局鼠标事件监听器

signals:
    void mouseSignal(QEvent* event);

private:
    GlobalMouseEvent(){}
};
#endif // MOUSEEVENT_H
