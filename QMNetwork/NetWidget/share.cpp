#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif
#include "share.h"

#include <QNetworkInterface>
#include <qhostinfo.h>

/**
 * @brief  获取本机IPv4地址
 * @return
 */
QString Share::getLocalIP()
{
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());

    for(auto address : info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            return address.toString();
        }
    }

    return "0.0.0.0";
}

QStringList Share::getLocalIPs()
{
    QStringList ips;
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());

    for(auto address : info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ips.append(address.toString());
        }
    }
    return ips;
}


/**
 * @brief    返回IP地址及其网卡类型
 * @return   <IP地址,网卡类型> 网卡类型  取值为[0,1],0表示无线，1表示有线
 */
QMap<QString, int> Share::getLocalCardIP()
{
    // 获取所有网络接口
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    QList<QNetworkAddressEntry> entry;
    QMap<QString, int> mapIP;
    foreach(QNetworkInterface inter, interfaces)
    {
        // 过滤掉vmware虚拟网卡和回环网卡
        if (networkCardType(inter.humanReadableName()))
            continue;

        if (inter.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning))
        {
            entry = inter.addressEntries();
            int cnt = entry.size() - 1;
            for (int i = 1; i <= cnt; ++i)
            {
                if (entry.at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    if (-1 != inter.name().indexOf("wireless"))
                    {
                        mapIP.insert(entry.at(i).ip().toString(), 0);
                    }
                    else if (-1 != inter.name().indexOf("ethernet"))
                    {
                        mapIP.insert(entry.at(i).ip().toString(), 1);
                    }
                }
            }
            entry.clear();
        }
    }
    return mapIP;
}

/**
 * @brief             检测当前网卡是否是虚拟网卡(VMware/VirtualBox)或回环网卡
 * @param strCardName 网卡的描述信息
 * @return            如果是虚拟网卡或回环网卡，返回true, 否则返回false
 */
bool Share::networkCardType(const QString &strCardName)
{
    if (-1 != strCardName.indexOf("VMware")
    || -1 != strCardName.indexOf("Loopback")
    || -1 != strCardName.indexOf("VirtualBox"))
        return true;

    return false;
}
