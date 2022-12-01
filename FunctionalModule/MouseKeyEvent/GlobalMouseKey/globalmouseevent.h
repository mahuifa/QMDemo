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

class QMouseEvent;
class QWheelEvent;

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
    /**
     * @brief 由于传递的是指针，为了保证不会出现内存泄露，需要在槽函数中delete。
     *        建议此信号只绑定一次，因为如果绑定多次可能会出现一个槽函数里把信号delete了，另外一个槽函数还在使用，出现野指针，或者多个槽函数多次delete
     */
    void mouseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    GlobalMouseEvent(){}
};
#endif // MOUSEEVENT_H
