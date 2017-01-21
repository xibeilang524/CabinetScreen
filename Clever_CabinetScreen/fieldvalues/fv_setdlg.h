#ifndef FV_SETDLG_H
#define FV_SETDLG_H

#include "op_outputdlg.h"

namespace Ui {
    class FV_SetDlg;
}

class FV_SetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FV_SetDlg(QWidget *parent = 0);
    ~FV_SetDlg();

    QVector <short> m_curMin; // 电流下限
    QVector <short> m_curMax; // 电流上限

    void setOutputBit(int bit);    
    void initData(QVector <short>&, QVector <short>&);
    void getData(QVector <short>&, QVector <short>&);

protected:
    void initEditBtn(void);
    void initBackground(void);

    void initKey(void);
    bool checkCurValue(void);
    void setCurValue(void);

    void checkAlarm(int bit);

protected slots:
    void editBtnPress(int value);
    void keyPress(int value);
    void timeoutDone(void);

private slots:
    void on_btn_enter_clicked();
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::FV_SetDlg *ui;

    int m_editBtn, m_bit;
    QSignalMapper *m,*sm;
    QTimer *timer;
};

#endif // FV_SETDLG_H
