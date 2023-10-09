#---------------------------------------------------------------------------------------
# 功能：      常用Qt功能示例代码
# 编译器：
#
# @开发者     mhf
# @邮箱       1603291350@qq.com
# @时间       2022/03/27
# @备注       在整个项目中采用了QMAKE_CFLAGS += /utf-8的方式指定编码格式，
#            如果是使用msvc编译器的话则可能需要版本不低于2017
#---------------------------------------------------------------------------------------
TEMPLATE = subdirs

SUBDIRS += FunctionalModule               # 常用功能模块，开发时直接复用
SUBDIRS += Experience                     # 日常开发经验示例出现
SUBDIRS += OpenGLDemo                     # OpenGL开发Demo
SUBDIRS += FFmpegDemo                     # ffmpeg库使用Demo
SUBDIRS += GraphicsviewDemo               # 图形视图框架使用Demo
SUBDIRS += OpenCVDemo                     # opencv常用功能Demo
SUBDIRS += ProgramFramework               # 简单的一些程序架构
SUBDIRS += QStyleDemo                     # Qt界面样式Demo
SUBDIRS += QMNetwork                      # Qt网络通信程序Demo
SUBDIRS += XlsxDemo                       # Qt使用QXlsx读写Excel Demo
SUBDIRS += QtChartsDemo                   # Qt使用QtCharts绘制图表 Demo
SUBDIRS += QSqlDemo                       # Qt使用数据库Demo
SUBDIRS += PaintingDemo                   # 使用 QPainter绘制图片
SUBDIRS += Web                            # Qt + web编程
SUBDIRS += ConcurrentExamples             # Qt并发编程示例
SUBDIRS += MapExamples                    # Qt + 地图开发示例

