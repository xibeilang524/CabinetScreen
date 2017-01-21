/*
 * db_sqlquery.cpp
 *
 *  Created on: 2016年6月1日
 *      Author: Lzy
 */
#include "db_sqlquery.h"


/**
 * @brief 创建纪录表
 * @return
 */
bool db_createRecordTable(void)
{
    QSqlQuery query;
    bool ret =  query.exec("create table recordLog (id int primary key,date vchar, time vchar, type vchar, description vchar)");
    //   if(ret == false)
    //       qDebug() << "create Record Table err";

    return ret;
}


/**
 * @brief 数据库连接
 * @return
 */
bool db_connection(void)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if(!db.open())
        return false;

    db_createRecordTable();
    return true;
}


DB_SqlQuery::DB_SqlQuery()
{

}




int DB_SqlQuery::tableSize(QString &name)
{
    QString cmd = "select * from "+name;
    int num = -1;

    bool ret = query.exec(cmd);
    if(ret)
    {
        if(query.last())
            num = query.value(0).toInt();
    }
    else
        qDebug() << cmd << "err";

    return num;
}

/**
 * @brief 插入操作纪录
 * @param log
 * @return
 */
bool DB_SqlQuery::insertRecord(sql_sRecordLog &log)
{
    bool ret = false;
    QString table = LOG_RECORD_TABLE;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("yyyy-MM-dd");
    QString time = dateTime.toString("hh:mm:ss");

    int num = tableSize(table);

    query.prepare("insert into recordLog (id, date,time,  type, description) "
                  "values (:id, :date, :time,  :type, :description)");
    query.bindValue(0, num+1);
    query.bindValue(1, date);
    query.bindValue(2, time);
    query.bindValue(3, log.type);
    query.bindValue(4, log.msg);
    ret = query.exec();
    if(!ret)
        qDebug() << "insert Record err";

    return  ret;
}


void DB_SqlQuery::selectTable(QString &name)
{
    QString cmd = "select * from "+name;
    bool ret = query.exec(cmd);
    if(ret)
    {
        while(query.next())
        {
            qDebug() << query.value(0).toInt() << query.value(1).toString();
        }
    }
    else
        qDebug() << cmd << "Err";
}

/**
  * @brief 删除日志纪录表
  * @return
  */
bool DB_SqlQuery::delRecordTable(void)
{
    QString cmd = "drop table recordLog";
    bool ret = query.exec(cmd);
    if(ret)
        ret = db_createRecordTable();
    else
        qDebug() << cmd << "Err";
    return ret;
}


bool sql_insert_record(sql_sRecordLog &log)
{
    DB_SqlQuery sql;

    return sql.insertRecord(log);
}
