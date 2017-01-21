/**
  * 传感器值显示窗口
  *         Lzy       2016-6-1
  */
#include "env_sensordlg.h"
#include "ui_env_sensordlg.h"


ENV_SensorDlg::ENV_SensorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ENV_SensorDlg)
{
    ui->setupUi(this);
    QTimer::singleShot(5,this,SLOT(InitFunction()));

    ui->setBtn->setEnabled(false);
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}

ENV_SensorDlg::~ENV_SensorDlg()
{
    delete ui;
}

/**
   * 功　能：初始化函数
   * 入口参数：
   * 返回值：无
   */
void ENV_SensorDlg::InitFunction(void)
{
    ui->temLcd->display("---");
    ui->temLcd_2->display("---");
    ui->temLcd_3->display("---");
    ui->temLcd_4->display("---");

    ui->humLcd->display("---");
    ui->humLcd_2->display("---");
    ui->humLcd_3->display("---");
    ui->humLcd_4->display("---");

    timeoutDone();
}

/**
   * 功　能：更新显示
   * 入口参数：
   * 返回值：无
   */
void ENV_SensorDlg::updateData(void)
{
    QString str;
    short temp=0;

    if(m_sensorData.hum.size() > 0)
    {
        temp = m_sensorData.temp.at(0) ;
        if(temp)
            ui->temLcd->display(temp);
        else
            ui->temLcd->display("---");

        temp = m_sensorData.temp.at(1) ;
        if(temp)
            ui->temLcd_2->display(temp);
        else
            ui->temLcd_2->display("---");

        temp = m_sensorData.temp.at(2) ;
        if(temp)
            ui->temLcd_3->display(temp);
        else
            ui->temLcd_3->display("---");

        temp = m_sensorData.temp.at(3) ;
        if(temp)
            ui->temLcd_4->display(temp);
        else
            ui->temLcd_4->display("---");

        temp = m_sensorData.hum.at(0) ;
        if(temp)
            ui->humLcd->display(temp);
        else
            ui->humLcd->display("---");

        temp = m_sensorData.hum.at(1) ;
        if(temp)
            ui->humLcd_2->display(temp);
        else
            ui->humLcd_2->display("---");

        temp = m_sensorData.hum.at(2) ;
        if(temp)
            ui->humLcd_3->display(temp);
        else
            ui->humLcd_3->display("---");

        temp = m_sensorData.hum.at(3) ;
        if(temp)
            ui->humLcd_4->display(temp);
        else
            ui->humLcd_4->display("---");
    }

    if(m_sensorData.door.size() > 0)
    {
        temp = m_sensorData.door.at(0);
        switch(temp)
        {
        case 0:
            str = tr("未启用");
            break;
        case 1:
            str = tr("打开");
            break;
        case 2:
            str = tr("关闭");
            break;
        case 3:
            str = tr("报警");
        }
        ui->door->setText(str);

        temp = m_sensorData.door.at(1);
        switch(temp)
        {
        case 0:
            str = tr("未启用");
            break;
        case 1:
            str = tr("打开");
            break;
        case 2:
            str = tr("关闭");
            break;
        case 3:
            str = tr("报警");
        }
        ui->door_2->setText(str);
    }

    if(m_sensorData.water.size() > 0)
    {
        temp = m_sensorData.water.at(0);
        switch(temp)
        {
        case 0:
            str = tr("未启用");
            break;
        case 1:
            str = tr("正常");
            break;
        case 2:
            str = tr("报警");
        }
        ui->water->setText(str);
    }

    if(m_sensorData.smoke.size() > 0)
    {
        temp = m_sensorData.smoke.at(0);
        switch(temp)
        {
        case 0:
            str = tr("未启用");
            break;
        case 1:
            str = tr("正常");
            break;
        case 2:
            str = tr("报警");
        }
        ui->smoke->setText(str);
    }

}

/**
   * 功　能：定时函数
   * 入口参数：
   * 返回值：无
   */
void ENV_SensorDlg::timeoutDone(void)
{
    m_sensorData = npm_get_pduData()->alarm; // 获取数据
    updateData();

    if(m_sensorData.humMax.size() > 0)
        ui->setBtn->setEnabled(true);
}

/**
   * 功　能：弹出设置阀值窗口
   * 入口参数：
   * 返回值：无
   */
void ENV_SensorDlg::on_setBtn_clicked()
{
    ENV_SenSetDlg *dlg = new ENV_SenSetDlg(this);
    dlg->exec();

    delete dlg;
}
