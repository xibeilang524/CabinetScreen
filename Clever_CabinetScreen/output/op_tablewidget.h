#ifndef OP_TABLEWIDGET_H
#define OP_TABLEWIDGET_H

#include <QtCore>
#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QtGui/QHeaderView>
#include "total_maindlg.h"

class OP_TableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OP_TableWidget(QWidget *parent = 0);

     void initTable(int base,int rows=12);
     void updateData(int ,NPM_sCurKwh &, QVector <short> &);

private:
    void initWidget(void);
    void addItemContent(int , int , QString &);
    void addRowContent(QStringList &list);

    void updateWidget(void);
    void checkAlarm(int bit, QTableWidgetItem *item);
signals:

public slots:
    void timeoutDone(void);

private:
    QTableWidget *m_tableWidget;
    QGridLayout *layout;
    QVector<QTableWidgetItem *> m_itemVec;

    int m_rows;
    int m_base;
    NPM_sCurKwh m_data;
    QVector <short> m_alarmflag;
};

#endif // OP_TABLEWIDGET_H
