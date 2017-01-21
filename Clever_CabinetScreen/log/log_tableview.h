#ifndef LOG_TABLEVIEW_H
#define LOG_TABLEVIEW_H

#include "QMessageBox"
#include "db_sqltablemodel.h"
#include <QTableView>
#include <QHeaderView>
#include <QGridLayout>

class LOG_TableView : public QWidget
{
    Q_OBJECT
public:
    explicit LOG_TableView(QWidget *parent = 0);
    ~LOG_TableView();

    void initRecordTable(void);

protected:
    bool refreshTable(QString &);
    int getMsgBox(QString &str);

signals:

public slots:
    void doubleSlot(QModelIndex);
    void refreshSlot(void);
    void delBtnSlot(void);
    void clearBtnSlot(void);

private:
    QTableView *view;
    QGridLayout *layout;
    DB_SqlTableModel *model;

    QString m_table;
};
extern void com_setBackColour(const QString &,QWidget *target);
#endif // LOG_TABLEVIEW_H
