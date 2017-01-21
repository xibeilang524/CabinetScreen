/**
  * 等待窗口
  *         Lzy       2016-6-1
  */
#include "env_waitdlg.h"
#include "ui_env_waitdlg.h"

ENV_WaitDlg::ENV_WaitDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ENV_WaitDlg)
{
    ui->setupUi(this);

    m_time = 0;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}

ENV_WaitDlg::~ENV_WaitDlg()
{
    delete ui;
}


void ENV_WaitDlg::timeoutDone(void)
{
    ui->progressBar->setValue(m_time++); // 进度条
    if(m_time > 31) // 等待时间
        this->close();
}
