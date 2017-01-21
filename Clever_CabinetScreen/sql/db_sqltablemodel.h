#ifndef DB_SQLTABLEMODEL_H
#define DB_SQLTABLEMODEL_H

#include <QWidget>
#include "db_sqlquery.h"

class DB_SqlTableModel : public QWidget
{
    Q_OBJECT
public:
    explicit DB_SqlTableModel(QWidget *parent = 0);

    void setHeaders(QStringList &);

    bool removeRow(int);
    bool refreshTable(QString &);

    void ascendingOrder(int);
    void descendingOrder(int);

protected:
    bool submitChanges(void);
    void revertChanges(void);

signals:

public slots:
    void queryFilter(QString);

public:
    QSqlTableModel *model;

};

#endif // DB_SQLTABLEMODEL_H
