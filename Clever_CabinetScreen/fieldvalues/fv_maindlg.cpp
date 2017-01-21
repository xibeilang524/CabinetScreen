#include "fv_maindlg.h"
#include "ui_fv_maindlg.h"

FV_MainDlg::FV_MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FV_MainDlg)
{
    ui->setupUi(this);    
    com_setBackColour(tr("阀值设置"),this);
}

FV_MainDlg::~FV_MainDlg()
{
    delete ui;
}

void FV_MainDlg::on_totalBtn_clicked()
{
    TOTAL_ThresholdDlg *dlg = new TOTAL_ThresholdDlg(this);
    dlg->exec();
    delete dlg;
}

void FV_MainDlg::on_outputBtn_clicked()
{
    FV_ThresholdDlg *dlg = new FV_ThresholdDlg(this);
    dlg->exec();

    delete dlg;
}
