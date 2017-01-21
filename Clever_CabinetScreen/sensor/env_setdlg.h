#ifndef ENV_SETDLG_H
#define ENV_SETDLG_H

#include "fv_maindlg.h"

namespace Ui {
    class ENV_SetDlg;
}

class ENV_SetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ENV_SetDlg(QWidget *parent = 0);
    ~ENV_SetDlg();

    short m_min,m_max;
    void initData(QString str, short min, short max);
    void updateData(short value);

protected:
    void initEditBtn(void);
    void initKey(void);
    void initBackground(void);

    bool checkCurValue(void);

protected slots:
    void editBtnPress(int value);
    void keyPress(int);
    void timeoutDone(void);

private slots:
    void on_okBtn_clicked();

    void on_btn_enter_clicked();

private:
    Ui::ENV_SetDlg *ui;

    short m_bit;
    int m_editBtn;
    QSignalMapper *sm, *m;
    QTimer *timer;
};

#endif // ENV_SETDLG_H
