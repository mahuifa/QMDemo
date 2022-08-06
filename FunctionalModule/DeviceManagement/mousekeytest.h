/******************************************************************************
 * @文件名       mousekeytest.h
 * @功能        鼠标、键盘热插拔监测
 *
 * @开发者     （作者）
 * @时间       2021/12/04
 * @备注
 *****************************************************************************/
#ifndef MOUSEKEYTEST_H
#define MOUSEKEYTEST_H

#include <QWidget>

namespace Ui {
class MouseKeyTest;
}

class MouseKeyTest : public QWidget
{
    Q_OBJECT

public:
    explicit MouseKeyTest(QWidget *parent = nullptr);
    ~MouseKeyTest();

private:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void registerGUID();

private:
    Ui::MouseKeyTest *ui;
};

#endif // MOUSEKEYTEST_H
