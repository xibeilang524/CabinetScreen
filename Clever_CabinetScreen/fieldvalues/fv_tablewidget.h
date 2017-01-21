#ifndef FV_TABLEWIDGET_H
#define FV_TABLEWIDGET_H

#include "fv_setdlg.h"

class FV_TableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FV_TableWidget(QWidget *parent = 0);
    ~FV_TableWidget();

    void initTable(int base,int rows=12);
    void updateData(NPM_sCurKwh &);
    void updateThreshold(int bit, short curMin, short curMax);
    void setUpdateFlag(void);

private:
   void initWidget(void);
   void addItemContent(int , int , QString &);
   void addRowContent(QStringList &list);

   void updateWidget(void);
   void initEditBtn(int base,int rows);
   void checkAlarm(int bit, QTableWidgetItem *item);

signals:
   void editBtnSig(int);

public slots:
   void timeoutDone(void);
   void editBtnPress(int);

private:
    QTableWidget *m_tableWidget;
    QGridLayout *layout;

    QSignalMapper *m;
    QVector<QTableWidgetItem *> m_itemVec;
    QVector<QPushButton *> m_setBtn;

    int m_rows;
    int m_base;
    bool m_oneTime;
    NPM_sCurKwh m_data;
};

#endif // FV_TABLEWIDGET_H
