#ifndef WID_OUTPUTBIT_H
#define WID_OUTPUTBIT_H

#include <QtCore>
#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QtGui/QHeaderView>
#include "mdb_npmvtype.h"

class WID_OutputBit : public QWidget
{
    Q_OBJECT
public:
    explicit WID_OutputBit(QWidget *parent = 0);
    
    void updateData(NPM_sCurKwh &data, QVector <short> &alarmflag);

signals:
    
public slots:
    void timeoutDone(void);

private:
    void initWidget(void);
    void addItemContent(int , int , QString &);
    void addRowContent(QStringList &list);
    void initTable(void);

    void updateWidget(void);
private:
    QTableWidget *m_tableWidget;
    QGridLayout *layout;
    QVector<QTableWidgetItem *> m_itemVec;

    NPM_sCurKwh m_data;
    QVector <short> m_alarmflag;
};

#endif // WID_OUTPUTBIT_H
