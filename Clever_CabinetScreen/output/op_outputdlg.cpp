/**
  *  输出位显示
  *         Lzy       2016-6-1
  */
#include "op_outputdlg.h"
#include "ui_op_outputdlg.h"

OP_OutputDlg::OP_OutputDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OP_OutputDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("输出电流值"),this);

#if defined(ARM_LINUX)
    setWindowState(Qt::WindowMaximized);
#endif

    m_table_1 = new OP_TableWidget(ui->groupBox);
    m_table_2 = new OP_TableWidget(ui->groupBox_2);

    m_table_1->initTable(0,12);
    m_table_2->initTable(12);

    QTimer::singleShot(5,this,SLOT(InitFunction()));  // 延时初始化
}

OP_OutputDlg::~OP_OutputDlg()
{
    delete ui;
}

/**
  * 功　能：初始化功能函数
  * 入口参数：
  * 返回值：
  */
void OP_OutputDlg::InitFunction(void)
{
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);

    timeoutDone();
}

/**
  * 功　能：定时响应函数
  * 入口参数：
  * 返回值：无
  */
void OP_OutputDlg::timeoutDone(void)
{
    NPM_sPDUData *pduData = npm_get_pduData();
    m_table_1->updateData(0,pduData->curKwh, pduData->alarm.alarmflag);
    m_table_2->updateData(12,pduData->curKwh, pduData->alarm.alarmflag);
}
