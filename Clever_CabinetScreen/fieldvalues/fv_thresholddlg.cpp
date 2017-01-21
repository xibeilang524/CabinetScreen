/**
  * 阀值设置主窗口
  *         Lzy       2016-6-2
  */
#include "fv_thresholddlg.h"
#include "ui_fv_thresholddlg.h"
#include "fv_waitdlg.h"

FV_ThresholdDlg::FV_ThresholdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FV_ThresholdDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("输出电流阀值"),this);

#if defined(ARM_LINUX)
    setWindowState(Qt::WindowMaximized);
#endif

    m_table_1 = new FV_TableWidget(ui->groupBox);
    connect(m_table_1,SIGNAL(editBtnSig(int)), this, SLOT(setSlot(int)));

    m_table_2 = new FV_TableWidget(ui->groupBox_2);
    connect(m_table_2,SIGNAL(editBtnSig(int)), this, SLOT(setSlot(int)));

    m_table_1->initTable(0,12);
    m_table_2->initTable(12);

    ui->okBtn->setEnabled(false);
    ui->setAllBtn->setEnabled(false);
    QTimer::singleShot(5,this,SLOT(InitFunction()));  // 延时初始化
    m_bit=0;
}

FV_ThresholdDlg::~FV_ThresholdDlg()
{
    delete ui;
}

/**
  * 功　能：设置阀值响应函数
  * 入口参数：bit -> 输出位
  * 返回值：
  */
void FV_ThresholdDlg::setSlot(int bit)
{
    FV_SetDlg *dlg = new FV_SetDlg(this);
    dlg->initData(m_curMin, m_curMax);
    dlg->setOutputBit(bit);

    int ret = dlg->exec();
    if(ret == QDialog::Accepted)
    {
        dlg->getData(m_curMin, m_curMax);
        if(bit) // 单独设置某位输出单元阀值
        {
            short curMin = m_curMin.at(bit-1);
            short curMax = m_curMax.at(bit-1);

            if(bit<12)
                m_table_1->updateThreshold(bit, curMin, curMax);
            else
                m_table_2->updateThreshold(bit-12, curMin, curMax);

            ui->okBtn->setEnabled( true);
        }
        else // 统一设置单元阀值
        {
            m_table_1->updateThreshold(bit, m_curMin.at(0), m_curMax.at(0));
            m_table_2->updateThreshold(bit, m_curMin.at(0), m_curMax.at(0));
            ui->okBtn->setEnabled( true);
        }

        m_bit = bit;
    }

    delete dlg;
}

/**
  * 功　能：初始化功能函数
  * 入口参数：
  * 返回值：
  */
void FV_ThresholdDlg::InitFunction(void)
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
void FV_ThresholdDlg::timeoutDone(void)
{
    NPM_sPDUData *pduData = npm_get_pduData();
    m_table_1->updateData(pduData->curKwh);
    m_table_2->updateData(pduData->curKwh);

    if(pduData->curKwh.curMax.size() > 0) // 没接收到数据不能设置
    {
        if(ui->setAllBtn->isEnabled() == false)
            ui->setAllBtn->setEnabled(true);
    }
}

/**
  * 功　能：统一设置响应函数
  * 入口参数：
  * 返回值：无
  */
void FV_ThresholdDlg::on_setAllBtn_clicked()
{
    setSlot(0); // 统一设置
}


/**
  * 功　能：日志保存
  * 入口参数：
  * 返回值：无
  */
 void FV_ThresholdDlg::slaveRecord(void)
 {
     QString str = tr("输出电流阀值修改");
     QString msg = "Output";

     if(m_bit)
      {
         msg += QString::number(m_bit);
         m_bit--;
     }
     msg += tr(" 修改范围:%1A~%2A").arg(m_curMin.at(m_bit)/10.0).arg(m_curMax.at(m_bit)/10.0);
     db_record_save(str,msg);
 }

void FV_ThresholdDlg::on_okBtn_clicked()
{
    NPM_SetUnit data;
    data.unit = NPM_CUR_MIN;
    data.data = m_curMin;
    npm_set_threshold(data);

    NPM_SetUnit datas;
    datas.unit = NPM_CUR_MAX;
    datas.data = m_curMax;
    npm_set_threshold(datas);

    slaveRecord();

    FV_WaitDlg *dlg = new  FV_WaitDlg(this);
    dlg->exec();
    delete dlg;

    on_refreshBtn_clicked();
    ui->okBtn->setEnabled(false);
}

/**
  * 功　能：阀值刷新
  * 入口参数：
  * 返回值：无
  */
void FV_ThresholdDlg::on_refreshBtn_clicked()
{
    m_table_1->setUpdateFlag();
    m_table_2->setUpdateFlag();

    m_curMin.clear();
    m_curMax.clear();

    ui->okBtn->setEnabled(false);
}

/**
  * 功　能：窗口退出
  * 入口参数：
  * 返回值：无
  */
void FV_ThresholdDlg::on_cancelBtn_clicked()
{
    bool ret = ui->okBtn->isEnabled();
    if(ret)
    {
        QMessageBox msgBox(this);
         com_setBackColour(tr("信息提示"),&msgBox);
        msgBox.setText(tr("\n您已修改阀值，是否要放弃修改？\n"));
        msgBox.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
        msgBox.setButtonText (QMessageBox::Ok,QString("确定"));
        msgBox.setButtonText (QMessageBox::Cancel,QString("取 消"));
        msgBox.setDefaultButton(QMessageBox::Ok);
        int res = msgBox.exec();
        if(res == QMessageBox::Ok)
            this->close();
    }
    else
        this->close();
}
