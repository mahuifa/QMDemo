#---------------------------------------------------------------------------------------
# @功能：      常用功能模块，开发时直接复用，避免重复造轮子
# @编译器：     Desktop Qt 5.12.5 MSVC2017 64bit（也支持其它编译器）
# @Qt IDE：    D:/Qt/Qt5.12.5/Tools/QtCreator/share/qtcreator
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022-08-06 08:22:56
# @备注
#---------------------------------------------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += WindowRect                     # 使用透明窗口框选鼠标所在窗口范围
SUBDIRS += SnippingTool                   # Qt实现截图工具
SUBDIRS += DeviceManagement               # 串口、鼠标、键盘热插拔检测模块
SUBDIRS += QLog                           # 自定义日志系统
SUBDIRS += NtpClient                      # NTP时间同步客户端（需要管理员权限/超级用户权限打开）
SUBDIRS += MouseKeyEvent                  # 自定义全局鼠标键盘事件监听器
SUBDIRS += QrCodeDemo                     # Qt封装qrencode的二维码生成、显示控件

win32 {
SUBDIRS += QMWidget                       # qt自定义窗口
SUBDIRS += QMPlayer                       # 视频播放器界面
}
msvc {
SUBDIRS += TestCrashHandler               # windows下Qt程序崩溃问题定位Demo（只在msvc编译下有效）
}
