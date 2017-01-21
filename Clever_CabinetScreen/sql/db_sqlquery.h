#ifndef DB_SQLQUERY_H
#define DB_SQLQUERY_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>

#define LOG_RECORD_TABLE    "recordLog"

// 日志结构体
typedef struct
{
    QString type;
    QString msg;
}sql_sRecordLog;

class DB_SqlQuery
{
public:
    DB_SqlQuery();

    void selectTable(QString &);
    bool insertRecord(sql_sRecordLog &);

    int tableSize(QString &);
    bool delRecordTable(void);

private:
     QSqlQuery query;
};

extern bool db_connection(void);
extern bool sql_insert_record(sql_sRecordLog &log);

#endif // DB_SQLQUERY_H
