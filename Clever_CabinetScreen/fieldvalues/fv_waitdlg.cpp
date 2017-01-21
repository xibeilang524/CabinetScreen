/**
  * 等待窗口
  *         Lzy       2016-6-2
  */
#include "fv_waitdlg.h"
#include "ui_fv_waitdlg.h"
extern void com_setBackColour(const QString &,QWidget *target);

FV_WaitDlg::FV_WaitDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FV_WaitDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("设置单元电流阀值"),this);

    m_time = 0;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}

FV_WaitDlg::~FV_WaitDlg()
{
    delete ui;
}

void FV_WaitDlg::timeoutDone(void)
{
    ui->progressBar->setValue(m_time++);
    if(m_time > 31)
        this->close();
}
