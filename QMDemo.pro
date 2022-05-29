#---------------------------------------------------------
# 功能：      常用Qt功能示例代码
# 编译器：
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/03/27
# @备注
#---------------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += QMWidget                       # qt自定义窗口
SUBDIRS += ProgramFramework               # 简单的一些程序架构
SUBDIRS += QStyleDemo                     # Qt界面样式Demo
SUBDIRS += DeviceManagement               # 串口、鼠标、键盘热插拔检测模块
SUBDIRS += QLog                           # 自定义日志系统
SUBDIRS += QMPlayer                       # 视频播放器界面
SUBDIRS += QMNetwork                      # Qt网络通信程序Demo
SUBDIRS += XlsxDemo                       # Qt使用QXlsx读写Excel Demo
SUBDIRS += QtChartsDemo                   # Qt使用QtCharts绘制图表 Demo
SUBDIRS += PlayImage                      # 使用 QPainter绘制图片
SUBDIRS += QSqlDemo                       # Qt使用数据库Demo

win32 {
SUBDIRS += TestCrashHandler               # windows下Qt程序崩溃问题定位Demo（只在msvc编译下有效）
}
