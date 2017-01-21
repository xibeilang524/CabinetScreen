#ifndef FV_WAITDLG_H
#define FV_WAITDLG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class FV_WaitDlg;
}

class FV_WaitDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FV_WaitDlg(QWidget *parent = 0);
    ~FV_WaitDlg();

public slots:
   void timeoutDone(void);

private:
    Ui::FV_WaitDlg *ui;

    int m_time;
    QTimer *timer;
};

#endif // FV_WAITDLG_H
