#ifndef FV_THRESHOLDDLG_H
#define FV_THRESHOLDDLG_H

#include "fv_tablewidget.h"

namespace Ui {
    class FV_ThresholdDlg;
}

class FV_ThresholdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FV_ThresholdDlg(QWidget *parent = 0);
    ~FV_ThresholdDlg();

protected slots:
    void InitFunction(void);
    void timeoutDone(void);
    void setSlot(int bit);
    void slaveRecord(void);

private slots:
    void on_setAllBtn_clicked();

    void on_okBtn_clicked();

    void on_refreshBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::FV_ThresholdDlg *ui;
     QTimer *timer;
    FV_TableWidget *m_table_1, *m_table_2;

    int m_bit;
    QVector <short> m_curMin; // 电流下限
    QVector <short> m_curMax; // 电流上限
};
#endif // FV_THRESHOLDDLG_H
