#ifndef GLOBALKEYEVENT_H
#define GLOBALKEYEVENT_H

#include <QObject>
class QKeyEvent;

/**
 *  全局鼠标事件单例信号类
 */
class GlobalKeyEvent : public QObject
{
    Q_OBJECT
public:
    static GlobalKeyEvent* getInstance()
    {
        static GlobalKeyEvent keyEvent;
        return &keyEvent;
    }

    static bool installKeyEvent();      // 安装全局键盘事件监听器
    static bool removeKeyEvent();       // 卸载全局键盘事件监听器

signals:
    /**
     * @brief 由于传递的是指针，为了保证不会出现内存泄露，需要在槽函数中delete。
     *        建议此信号只绑定一次，因为如果绑定多次可能会出现一个槽函数里把信号delete了，另外一个槽函数还在使用，出现野指针，或者多个槽函数多次delete
     */
    void keyEvent(QKeyEvent* event);

private:
    GlobalKeyEvent(){}
};

#endif // GLOBALKEYEVENT_H
