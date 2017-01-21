#ifndef TOTAL_WAITDLG_H
#define TOTAL_WAITDLG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class TOTAL_WaitDlg;
}

class TOTAL_WaitDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TOTAL_WaitDlg(QWidget *parent = 0);
    ~TOTAL_WaitDlg();

public slots:
   void timeoutDone(void);

private:
    Ui::TOTAL_WaitDlg *ui;

    int m_time;
    QTimer *timer;
};

#endif // TOTAL_WAITDLG_H
