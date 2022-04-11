/******************************************************************************
 * @文件名     share.h
 * @功能       包含一些常用的共有功能函数
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/04/10
 * @备注
 *****************************************************************************/
#ifndef SHARE_H
#define SHARE_H
#include <QString>
#include <QMap>

namespace Share
{
/**
 * @brief  返回1个本机IP地址
 * @return
 */
QString getLocalIP();
/**
 * @brief  返回所有本机IP地址
 * @return
 */
QStringList getLocalIPs();
/**
 * @brief  返回本机IP地址和网卡类型
 * @return
 */
QMap<QString, int> getLocalCardIP();

bool networkCardType(const QString& strCardName);
};

#endif // SHARE_H
