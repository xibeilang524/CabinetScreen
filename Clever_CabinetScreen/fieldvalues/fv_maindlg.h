#ifndef FV_MAINDLG_H
#define FV_MAINDLG_H

#include <QDialog>
#include "fv_thresholddlg.h"

namespace Ui {
    class FV_MainDlg;
}

class FV_MainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FV_MainDlg(QWidget *parent = 0);
    ~FV_MainDlg();

private slots:
    void on_totalBtn_clicked();

    void on_outputBtn_clicked();

private:
    Ui::FV_MainDlg *ui;
};

#endif // FV_MAINDLG_H
