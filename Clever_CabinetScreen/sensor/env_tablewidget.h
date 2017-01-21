#ifndef ENV_TABLEWIDGET_H
#define ENV_TABLEWIDGET_H

#include "env_setdlg.h"

class ENV_TableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ENV_TableWidget(QWidget *parent = 0);

    void updateData(NPM_sAlarm &data);
    void setUpdateFlag(void);
    void updateThreshold(int bit, short min, short max);

protected:
    void initWidget(void);
    void addItemContent(int row, int column, QString &content);
    void addRowContent(QStringList &list);
    void initTable(void);

    void updateWidget(void);
    void initEditBtn(int base,int value);

    void setTemp(int bit, short min, short max);
    void setHum(int bit, short min, short max);

signals:
    void editBtnSig(int);

public slots:
    void timeoutDone(void);
    void editBtnPress(int value);

private:
    QTableWidget *m_tableWidget;
    QGridLayout *layout;

    QSignalMapper *m;
    QVector<QTableWidgetItem *> m_itemVec;
    QVector<QPushButton *> m_setBtn;

    bool m_oneTime;
    NPM_sAlarm m_data;
};

#endif // ENV_TABLEWIDGET_H
