TEMPLATE = subdirs

# 只有当前系统、编译器支持webenginewidgets模块才编译下列程序（MinGW没有），这里 { 不能换行
qtHaveModule(webenginewidgets) {
    SUBDIRS += minimal   # 使用QWebEngineView加载显示Web页面的最小示例
}


