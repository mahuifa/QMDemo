# Qt使用QtSql模块操作数据库Demo说明及演示

[toc]



## 1、说明

| 工程                 | 功能                                                         |
| -------------------- | ------------------------------------------------------------ |
| TableModel           | 展示了使用QSqlTableModel来显示数据库信息                     |
| RelationalTableModel | 展示了使用QSqlRelationalTableModel来使用数据库[外键]         |
| SqlWidgetMapper      | 将数据库内容通过QDataWidgetMapper将模型数据映射到小部件      |
| CachedTable          | 通过QSqlTableModel显示和修改数据，通过按键保存修改内容和恢复修改内容 |



## 2、实现效果

### 1.1 TableModel

![image-20220518112255014](QSql.assets/image-20220518112255014.png)

### 1.2 RelationalTableModel

![RelationalTableModel](C:/Users/mhf/Desktop/RelationalTableModel.gif)

### 1.3 SqlWidgetMapper

![SqlWidgetMapper](QSql.assets/SqlWidgetMapper.gif)

### 1.4 CachedTable

![CachedTable](C:/Users/mhf/Desktop/CachedTable.gif)
