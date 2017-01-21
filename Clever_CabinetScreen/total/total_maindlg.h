#ifndef TOTAL_MAINDLG_H
#define TOTAL_MAINDLG_H

#include "total_thresholddlg.h"

namespace Ui {
    class TOTAL_MainDlg;
}

class TOTAL_MainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TOTAL_MainDlg(QWidget *parent = 0);
    ~TOTAL_MainDlg();

protected:
    void initWidget(void);
    void updateWidget(void);
    void checkAlarm(void);


protected slots:
    void InitFunction(void);
    void timeoutDone(void);

private slots:
    void on_line_1_clicked();

    void on_line_2_clicked();

    void on_line_3_clicked();

private:
    Ui::TOTAL_MainDlg *ui;

    QTimer *timer;
    NPM_sTotalData m_totalData;
};



#endif // TOTAL_MAINDLG_H
