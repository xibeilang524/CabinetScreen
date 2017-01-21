#ifndef ENV_WAITDLG_H
#define ENV_WAITDLG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class ENV_WaitDlg;
}

class ENV_WaitDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ENV_WaitDlg(QWidget *parent = 0);
    ~ENV_WaitDlg();

public slots:
   void timeoutDone(void);

private:
    Ui::ENV_WaitDlg *ui;

    int m_time;
    QTimer *timer;
};

#endif // ENV_WAITDLG_H
