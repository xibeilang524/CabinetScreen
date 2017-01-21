/**
  * 传感器阀值显示窗口
  *         Lzy       2016-6-1
  */
#include "env_sensetdlg.h"
#include "ui_env_sensetdlg.h"
#include "env_setdlg.h"
#include "env_waitdlg.h"

ENV_SenSetDlg::ENV_SenSetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ENV_SenSetDlg)
{
    ui->setupUi(this);

    m_envTable = new ENV_TableWidget(ui->groupBox);
    connect(m_envTable,SIGNAL(editBtnSig(int)), this, SLOT(setSlot(int)));

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
    ui->saveBtn->setEnabled(false);
}

ENV_SenSetDlg::~ENV_SenSetDlg()
{
    delete ui;
}


/**
   * 功　能：设置阀值 阀值会通过Modbus发送出去
   * 入口参数：
   * 返回值：无
   */
 void ENV_SenSetDlg::setData(void)
 {
     NPM_SetUnit data;
     data.unit = NPM_TH_TMIN;
     data.data = tempMin + humMin;
     npm_set_threshold(data);

     data.unit = NPM_TH_TMAX;
     data.data = tempMax + humMax;
     npm_set_threshold(data);
 }

 /**
    * 功　能：修改阀值
    * 入口参数：
    * 返回值：无
    */
void ENV_SenSetDlg::changeData(int value, short min, short max)
{
    int bit = value;
    if(value<10) // 修改温度
    {
        if(value == 0) // 统一修改
        {
            for(int i=0; i< NPMV_TH_COUNT; ++i)
            {
                tempMin[i] = min;
                tempMax[i] = max;
            }
        }
        else //  修改其中一个阀值
        {
            value--;
            tempMin[value] = min;
            tempMax[value] = max;
        }
    }
    else // 修改湿度
    {
        value -= 10;
        if(value == 0) // 统一修改湿度
        {
            for(int i=0; i< NPMV_TH_COUNT; ++i)
            {
                humMin[i] = min;
                humMax[i] = max;
            }
        }
        else // 修改其中一个湿度
        {
            value--;
            humMin[value] = min;
            humMax[value] = max;
        }
    }
}


/**
   * 功　能：阀值设置响应函数
   * 入口参数：value -> 修改各类代号
   * 返回值：无
   */
void ENV_SenSetDlg::setSlot(int value)
{
    QString str;
    short k=0,min, max;

    if(value < 10)
    {
        str = tr("温度");
        if(value > 0)
        {
             k = value -1;
            str += QString::number(value) ;
        }
        else
            k = 0;

        min = tempMin.at(k);
        max = tempMax.at(k);
    }
    else
    {
         str = tr("湿度");
        k =  value -10;
        if(k > 0)
            str += QString::number(k--) ;
        else
            k = 0;
        min = humMin.at(k);
        max = humMax.at(k);
    }

    ENV_SetDlg *dlg = new ENV_SetDlg(this);
    dlg->initData(str,min, max);
    dlg->updateData(value);

    int ret = dlg->exec();
    if(ret == QDialog::Accepted)
    {
        min = dlg->m_min;
        max = dlg->m_max;
        changeData(value,min , max);

        ui->saveBtn->setEnabled(true);
        m_envTable->updateThreshold(value, min, max);
    }

    delete dlg;

}


void ENV_SenSetDlg::timeoutDone(void)
{
    NPM_sPDUData *pduData = npm_get_pduData();
    m_envTable->updateData(pduData->alarm);

    if(humMax.size() == 0)
    {
        tempMin = pduData->alarm.tempMin;
        humMin = pduData->alarm.humMin;

        tempMax = pduData->alarm.tempMax;
        humMax = pduData->alarm.humMax;
    }
}


/**
   * 功　能：阀值显示刷新
   * 入口参数：
   * 返回值：无
   */
void ENV_SenSetDlg::on_refreshBtn_clicked()
{
    m_envTable->setUpdateFlag();
    humMax.clear();
    timeoutDone();
}

/**
   * 功　能：保存按键响应函数
   * 入口参数：
   * 返回值：无
   */
void ENV_SenSetDlg::on_saveBtn_clicked()
{
    setData();
    ui->saveBtn->setEnabled(false);

    ENV_WaitDlg *dlg = new ENV_WaitDlg(this);
    dlg->exec();
    delete dlg;
}

/**
   * 功　能：退出按键响应函数
   * 入口参数：
   * 返回值：无
   */
void ENV_SenSetDlg::on_quitBtn_clicked()
{
    bool ret = ui->saveBtn->isEnabled();
    if(ret)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("信息提示");
        msgBox.setText(tr("您已修改阀值，是否要放弃修改？"));
        msgBox.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
        msgBox.setButtonText (QMessageBox::Ok,QString("确定"));
        msgBox.setButtonText (QMessageBox::Cancel,QString("取 消"));
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int res = msgBox.exec();
        if(res == QMessageBox::Ok)
            this->close();
    }
    else
        this->close();
}

/**
   * 功　能：统一修改温度
   * 入口参数：
   * 返回值：无
   */
void ENV_SenSetDlg::on_tempBtn_clicked()
{
    setSlot(0);
}

/**
   * 功　能：统一修改湿度
   * 入口参数：
   * 返回值：无
   */
void ENV_SenSetDlg::on_humBtn_clicked()
{
    setSlot(10);
}
