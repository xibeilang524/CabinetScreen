#include "total_waitdlg.h"
#include "ui_total_waitdlg.h"
extern void com_setBackColour(const QString &,QWidget *target);

TOTAL_WaitDlg::TOTAL_WaitDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TOTAL_WaitDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("设置总电流阀值"),this);

    m_time = 0;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}

TOTAL_WaitDlg::~TOTAL_WaitDlg()
{
    delete ui;
}


void TOTAL_WaitDlg::timeoutDone(void)
{
    ui->progressBar->setValue(m_time++); // 进度条
    if(m_time > 21) // 等待时间
        this->close();
}
