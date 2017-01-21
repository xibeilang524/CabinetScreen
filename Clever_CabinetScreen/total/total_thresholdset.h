#ifndef TOTAL_THRESHOLDSET_H
#define TOTAL_THRESHOLDSET_H

#include <QtCore>
#include <QDialog>
#include <QMessageBox>
#include "mdb_corethread.h"

namespace Ui {
    class TOTAL_ThresholdSet;
}

class TOTAL_ThresholdSet : public QDialog
{
    Q_OBJECT

public:
    explicit TOTAL_ThresholdSet(QWidget *parent = 0);
    ~TOTAL_ThresholdSet();


    void setData(QVector <short> &min, QVector <short> &max);
    void getData(QVector <short> &min, QVector <short> &max);

protected:
    void initKey(void);
    void initEditBtn(void);
    void initBackground(void);

    void initWidget(void);
    void updateData(void);

    void setCurValue(void);
    bool checkCurValue(void);
    void checkAlarm(void);

signals:
    void setThresholdSig();

private slots:
    void keyPress(int);
     void editBtnPress(int);
    void InitFunction(void);
    void timeoutDone(void);

    void on_btn_enter_clicked();

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::TOTAL_ThresholdSet *ui;
    QSignalMapper *m, *sm;

    int m_editBtn;
    QTimer *timer;
    NPM_sTotalData m_totalData;
    QVector <short> m_tcurMin; // 总电流下限
    QVector <short> m_tcurMax; // 总电流上限
};
extern NPM_sPDUData *npm_get_pduData(void);
#endif // TOTAL_THRESHOLDSET_H
