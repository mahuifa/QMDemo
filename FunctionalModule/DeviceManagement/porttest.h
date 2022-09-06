/******************************************************************************
 * @文件名       porttest.h
 * @功能        串口检测功能测试，1、获取所有可用串口名； 2、自动监测串口热插拔
 *
 * @开发者     （作者）
 * @时间       2021/12/04
 * @备注
 *****************************************************************************/
#ifndef PORTTEST_H
#define PORTTEST_H

#include <QWidget>

namespace Ui {
class PortTest;
}

class PortTest : public QWidget
{
    Q_OBJECT

public:
    explicit PortTest(QWidget *parent = nullptr);
    ~PortTest();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    void on_comStatus(QString name, bool flag);

private:
    Ui::PortTest *ui;
};

#endif // PORTTEST_H
