#ifndef WID_TOTAL_H
#define WID_TOTAL_H

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QGridLayout>
#include <QTimer>
#include "mdb_npmvtype.h"

class WID_Total : public QWidget
{
    Q_OBJECT
public:
    explicit WID_Total(QWidget *parent = 0);
    
    void updateData(NPM_sTotalData &data);

protected:
    void createControl(void);
    void layoutControl(void);

    void updateWidget(void);
signals:
    
public slots:
    void onRadio1Clicked();
    void onRadio2Clicked();
    void onRadio3Clicked();

    void timeoutDone(void);

private:
    QLabel *lab;
    QLabel *curLab, *cur;
    QLabel *volLab, *vol;
    QLabel *tpfLab, *tpf;
    QLabel *kwhLab, *kwh;

    QRadioButton *radio1,*radio2,*radio3;

    int m_line;
    NPM_sTotalData m_totalData;
};

#endif // WID_TOTAL_H
