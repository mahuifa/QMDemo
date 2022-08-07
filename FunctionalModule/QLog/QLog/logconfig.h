/******************************************************************************
* @文件名     logconfig.h
* @功能      日志模块配置文件管理类，负责创建配置文件，读取配置文件信息
*
* @开发者     mhf
* @邮箱      1603291350@qq.com
* @时间      2022/03/27
* @备注
*****************************************************************************/
#ifndef LOGCONFIG_H
#define LOGCONFIG_H

#include <QObject>

namespace Config {
enum TxtRelyMode        // TXT保存日志创建新文件方式
{
    Time,               // 以时间为依据创建（如12小时、24小时）
    Size,               // 以文件大小为依据创建（如1M、10M）
    RowNum              // 以日志行数为依据创建（如1000行、10000行）
};
typedef struct
{
    TxtRelyMode relyMode;    // 创建文件模式
    int time;                // 保存时间（relyMode 为Time时生效，有12、24两种）
    uint size;               // 文件大小(单位Mb)
    uint rowNum;             // 文件行数
    QString name;            // 日志文件名
}TxtConfig;                  // txt保存日志配置信息
}

using namespace Config;
class LogConfig
{
public:
    LogConfig();

    static void init();
    static void initTxtConfig();
    static void setTxtLogName(QString name);

public:
    static TxtConfig txtConfig;

};

#endif // LOGCONFIG_H
