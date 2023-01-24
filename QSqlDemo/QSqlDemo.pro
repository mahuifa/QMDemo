TEMPLATE = subdirs

SUBDIRS += TableModel              # (1) 这个Demo中展示了如何使用具有表视图的专用 SQL 表模型（QSqlTableModel）来编辑数据库中的信息。
SUBDIRS += TableModel2             # (2) 这个Demo中展示了如何使用具有表视图的专用 SQL 表模型（QSqlTableModel）来编辑数据库中的信息，实现创建空白数据行、自增key
SUBDIRS += CachedTable             # (3) 通过QSqlTableModel显示和修改数据，通过按键保存修改内容和恢复修改内容
SUBDIRS += RelationalTableModel    # (4) 本示例展示了如何使用QSqlRelationalTableModel（关系表模型）来可视化数据库中[外键]的使用。
SUBDIRS += SqlWidgetMapper         # (5) 通过QDataWidgetMapper将数据库数据映射到小部件
SUBDIRS += SignIn                  # (6) 使用QSqlite数据库实现用户登录、后台管理用户功能
