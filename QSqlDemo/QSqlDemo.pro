TEMPLATE = subdirs

SUBDIRS += \
    CachedTable \            # 通过QSqlTableModel显示和修改数据，通过按键保存修改内容和恢复修改内容
    RelationalTableModel \   # 本示例展示了如何使用QSqlRelationalTableModel（关系表模型）来可视化数据库中[外键]的使用。
    SqlWidgetMapper \        # 通过QDataWidgetMapper将数据库数据映射到小部件
    TableModel \             # 这个Demo中展示了如何使用具有表视图的专用 SQL 表模型（QSqlTableModel）来编辑数据库中的信息。
    TableModel2              # 这个Demo中展示了如何使用具有表视图的专用 SQL 表模型（QSqlTableModel）来编辑数据库中的信息，实现创建空白数据行、自增key
