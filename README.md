<img src="README.assets/Qt-5.12.12-green.png">[![个人主页-CSDN-orange](README.assets/%E4%B8%AA%E4%BA%BA%E4%B8%BB%E9%A1%B5-CSDN-orange.png)](https://blog.csdn.net/qq_43627907?type=blog)[![国内站点-gitee-orange](README.assets/%E5%9B%BD%E5%86%85%E7%AB%99%E7%82%B9-gitee-orange.png)](https://gitee.com/mahuifa/QMDemo)[![国外站点-github-brightgreen](README.assets/%E5%9B%BD%E5%A4%96%E7%AB%99%E7%82%B9-github-brightgreen.png)](https://github.com/mahuifa/QMDemo)

![image-20220827211417219](README.assets/image-20220827211417219.png)



<table><tr><td bgcolor=#FFFF00>在这个仓库中收录了一些自己练习Qt用的Demo和日常开发中经常用到的功能模块。</td></tr></table>

# 一、工程模块说明

|       模块名       | 功能                                                         |
| :----------------: | ------------------------------------------------------------ |
|  FunctionalModule  | [自己封装的常用功能模块，开发时直接复用](./FunctionalModule/FunctionalModule.md) |
|  ProgramFramework  | [用过的一些Qt程序模块设计架构Demo](./ProgramFramework/ProgramFramework.md) |
|     Experience     | [日常开发经验](./Experience/Experience.md)                   |
|                    |                                                              |
|                    | **使用Qt内置模块**                                           |
|     QMNetwork      | [Qt网络编程Demo](./QMNetwork/QMNetwork.md)                   |
|    QtChartsDemo    | [Qt使用QtCharts模块绘制图表Demo](./QtChartsDemo/QtCharts.md) |
|      QSqlDemo      | [QSql学习Demo](./QSqlDemo/QSql.md)                           |
|     QStyleDemo     | [Qt样式美化设置Demo说明及演示](./QStyleDemo/QStyleDemo.md)   |
|    PaintingDemo    | [Qt绘图Demo](./PaintingDemo/PaintingDemo.md)                 |
|  GraphicsviewDemo  | [GraphicsView 图形视图框架Demo](./GraphicsviewDemo/GraphicsviewDemo.md) |
|      QOpenGL       | [Qt - OpenGL开发](./OpenGLDemo/OpenGLDemo.md)                |
|        Web         | [Qt - Web混合开发](./Web/Web.md)                             |
| ConcurrentExamples | [Qt线程和并发编程示例](./ConcurrentExamples/ConcurrentExamples.md) |
|    MapExamples     | [Qt 地图功能开发](./MapExamples/MapExamples.md)              |
|                    |                                                              |
|                    | **使用到第三方库**                                           |
|  Web/CefExamples   | [Qt - Web混合开发（使用CEF）](./Web/Web.md)                  |
|      XlsxDemo      | [Qt使用QXlsx操作Excel Demo](./XlsxDemo/XlsxDemo.md)          |
|     OpenCVDemo     | [Qt使用OpenCV进行视频、图像操作Demo](./OpenCVDemo/OpenCVDemo.md) |
|     FFmpegDemo     | [Qt - FFmpeg多媒体框架开发Demo](./FFmpegDemo/FFmpegDemo.md)  |



# 二、构建说明

* 使用到第三方库的项目都会依赖于第三方库，所以直接下载源码编译是无法编译使用了第三方库的工程的；
* 需要打开对应工程的说明文件，修改依赖文件的路径，然后才可以编译；
* 并且使用因为程序依赖于其它文件，所以即使编程成功了也不一定能成功运行；
* 需要将依赖的文件/动态库拷贝到可执行程序路径下才可以正常运行；
