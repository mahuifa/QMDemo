#ifndef GLOBALKEYEVENT_H
#define GLOBALKEYEVENT_H

#include <QObject>
#include <QKeyEvent>

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
    void keyEvent(QKeyEvent event);

private:
    GlobalKeyEvent(){}
};

#endif // GLOBALKEYEVENT_H
