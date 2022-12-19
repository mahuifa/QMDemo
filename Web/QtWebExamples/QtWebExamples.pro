TEMPLATE = subdirs

# 只有当前系统、编译器支持webenginewidgets模块才编译下列程序（MinGW没有），这里 { 不能换行
qtHaveModule(webenginewidgets) {
    SUBDIRS += minimal               # 使用QWebEngineView加载显示Web页面的最小示例1
    SUBDIRS += minimal2              # 使用QWebEngineView加载显示Web页面的最小示例2
    SUBDIRS += authorization         # 使用QWebEnginePage打开摄像头演示权限管理
    SUBDIRS += channelDemo           # 使用QtWebChannel实现Qt与Web通信交互（简单示例）
    SUBDIRS += channelDemo2          # 使用QtWebChannel实现Qt与Web通信交互（进阶功能）
    SUBDIRS += channelWebSocket      # 使用QWebSocket作为QWebChannel通信数据传输接口，实现与远程网页交互功能
}


