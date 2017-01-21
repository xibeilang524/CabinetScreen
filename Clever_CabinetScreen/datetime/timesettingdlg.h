#ifndef TIMESETTINGDLG_H
#define TIMESETTINGDLG_H

#include <QDialog>
#include <QtCore>
#include <sys/time.h>

namespace Ui {
    class TimeSettingDlg;
}

class TimeSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TimeSettingDlg(QWidget *parent = 0);
    ~TimeSettingDlg();

protected:
    bool isLeap(int );

    void initBackground(void);
    void initEditBtn(void);
    void initKey(void);

protected slots:
    void editBtnPress(int);
    void keyPress(int value);
    void dateSetLimit(int);
    void timeSetup(void);

private slots:
    void on_btn_enter_clicked();

    void on_quitBtn_clicked();

private:
    Ui::TimeSettingDlg *ui;

    QSignalMapper *sm,*m;
    int preYear, preMonth;
    int m_editBtn;
};

#endif // TIMESETTINGDLG_H
