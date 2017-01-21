/**
  *  总电流、电压显示
  *         Lzy       2016-6-1
  */
#include "total_maindlg.h"
#include "ui_total_maindlg.h"

static short g_line = 0; // 相数


TOTAL_MainDlg::TOTAL_MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TOTAL_MainDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("电气参数"),this);

#if defined(ARM_LINUX)
    setWindowState(Qt::WindowMaximized);
#endif

    ui->groupBox->setHidden(true); // 只有一项，所以隐藏

    m_totalData = npm_get_pduData()->totalData; // 获取数据
    QTimer::singleShot(5,this,SLOT(InitFunction()));

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

TOTAL_MainDlg::~TOTAL_MainDlg()
{
    delete ui;
}


/**
  * 功　能：初始化Widget
  * 入口参数：
  * 返回值：无
  */
void TOTAL_MainDlg::initWidget(void)
{
    QString str = "0.0";
    ui->curLcd->setDigitCount(4);
    ui->curLcd->display(str);
    ui->powerLcd->display(str);

    str = "0";
    ui->volLcd->setDigitCount(3);
    ui->volLcd->display(str);

    str = "0.00";
    ui->tpfLcd->setDigitCount(4);
    ui->tpfLcd->display(str);

    str = "0";
    ui->kwhLcd->display(str);

    if(g_line == 0)
        ui->line_1->setChecked(true);
    else  if(g_line == 1)
        ui->line_2->setChecked(true);
    else  if(g_line == 2)
        ui->line_3->setChecked(true);
}


void TOTAL_MainDlg::updateWidget(void)
{    
    float temp = m_totalData.Tcur.at(g_line) / 10.0;
    QString str = QString::number(temp,'f', 1);
    ui->curLcd->display(str);

    temp = m_totalData.Tvol.at(g_line);
    str = QString::number(temp);
    ui->volLcd->display(str);

    temp = m_totalData.Tvol.at(g_line)  *  m_totalData.Tcur.at(g_line) / 10000.0; // 得到功率
    str = QString::number(temp,'f', 1);
    ui->powerLcd->display(str);

    temp = m_totalData.Tpf.at(g_line) /100.0;
    str = QString::number(temp,'f', 2);
    ui->tpfLcd->display(str);

    temp = m_totalData.TKwh.at(g_line) /10.0;
    str = QString::number(temp,'f', 1);
    ui->kwhLcd->display(str);
}

/**
  * 功　能：初始化入口函数
  * 入口参数：
  * 返回值：无
  */
void TOTAL_MainDlg::InitFunction(void)
{
    initWidget();
    timeoutDone();

    timer->start(1000);
}

void TOTAL_MainDlg::checkAlarm(void)
{
    extern bool alarm_line_cur(int line);

    bool ret = alarm_line_cur(0);
    if(ret)
        ui->curLcd->setStyleSheet("color: rgb(255, 0, 0);");
    else
        ui->curLcd->setStyleSheet("color:black;");
}

/**
  * 功　能：定时器响应函数
  * 入口参数：
  * 返回值：无
  */
void TOTAL_MainDlg::timeoutDone(void)
{
    m_totalData = npm_get_pduData()->totalData; // 获取数据
    if(m_totalData.TKwh.size() > 0) // 已经收取到数据
    {
        updateWidget();
        checkAlarm();
    }
}

void TOTAL_MainDlg::on_line_1_clicked()
{
    g_line = 0;
    timeoutDone();
}

void TOTAL_MainDlg::on_line_2_clicked()
{
    g_line = 1;
    timeoutDone();
}

void TOTAL_MainDlg::on_line_3_clicked()
{
    g_line = 2;
    timeoutDone();
}

