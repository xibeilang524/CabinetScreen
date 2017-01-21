/**
  * 阀值设置窗口
  *         Lzy       2016-6-2
  */
#include "fv_setdlg.h"
#include "ui_fv_setdlg.h"

FV_SetDlg::FV_SetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FV_SetDlg)
{
    ui->setupUi(this);
      com_setBackColour(tr("输出电流阀值设置"),this);

    m_editBtn = m_bit =0;
    ui->okBtn->setEnabled(false);

    initEditBtn();
    initKey();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}

FV_SetDlg::~FV_SetDlg()
{
    delete ui;
}


void FV_SetDlg::checkAlarm(int bit)
{
    extern bool alarm_output_bit(int bit);

    bool ret = alarm_output_bit(bit);
    if(ret)
        ui->curEdit->setStyleSheet("color: rgb(255, 0, 0);");
    else
        ui->curEdit->setStyleSheet("color:black;");
}


void FV_SetDlg::timeoutDone(void)
{
    if(m_bit)
    {
        NPM_sPDUData *pduData = npm_get_pduData();
        short cur = pduData->curKwh.cur.at(m_bit-1);
        QString str = QString::number(cur/ 10.0,'f', 1);
        ui->curEdit->setText(str);

        checkAlarm(m_bit-1);
    }
}

void FV_SetDlg::initData(QVector <short> &curMin, QVector <short> &curMax)
{
    if(curMin.size())
    {
        m_curMin = curMin;
        m_curMax = curMax;
    }
    else
    {
        NPM_sPDUData *pduData = npm_get_pduData();

        m_curMin = pduData->curKwh.curMin;
        m_curMax = pduData->curKwh.curMax;
    }
}

void FV_SetDlg::getData(QVector <short> &curMin, QVector <short> &curMax)
{
    curMin = m_curMin;
    curMax = m_curMax;
}

void FV_SetDlg::setOutputBit(int bit)
{
    QString str = "Output";
    m_bit = bit;

    if(bit==0)
        ui->curEdit->setEnabled(false);
    else
        str +=  QString::number(bit--);
    double temp = m_curMin.at(bit) / 10.0;
    ui->curMin->setValue(temp);

    temp = m_curMax.at(bit) / 10.0;
    ui->curMax->setValue(temp);

    ui->outputLabel->setText(str);
    timeoutDone();
}



/**
   * 功　能：初始化编辑按键
   * 入口参数：
   * 返回值：无
   */
void FV_SetDlg::initEditBtn(void)
{
    sm = new QSignalMapper(this);
    connect(ui->curMinBtn,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->curMaxBtn,SIGNAL(clicked()),sm,SLOT(map()));

    sm->setMapping(ui->curMinBtn,1);
    sm->setMapping(ui->curMaxBtn,11);

    connect(sm,SIGNAL(mapped(int)),this,SLOT(editBtnPress(int)));
}

/**
   * 功　能：初始化背景色
   * 入口参数：
   * 返回值：无
   */
void FV_SetDlg::initBackground(void)
{
    QString str =  "background-color:white;";
    ui->curMin->setStyleSheet(str);
    ui->curMax->setStyleSheet(str);
}

void FV_SetDlg::editBtnPress(int value)
{
    QDoubleSpinBox *spinBox=NULL;
    QString colour = "background-color:rgb(199,237,204)";

    m_editBtn = value;
    switch(value)
    {
    case 1:
        spinBox = ui->curMin;
        break;

    case 11:
        spinBox = ui->curMax;
        break;

    }
    initBackground();
    spinBox->setStyleSheet(colour);

    double temp = spinBox->value();
    QString str = QString::number(temp,'f', 1);

    ui->lineEdit->setText(str);
    ui->lineEdit->selectAll();
}

/**
   * 功　能：初始化键盘按键
   * 入口参数：
   * 返回值：无
   */
void FV_SetDlg::initKey(void)
{
    m = new QSignalMapper(this);
    connect(ui->btn_0,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_1,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_2,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_3,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_4,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_5,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_6,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_7,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_8,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_9,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_del,SIGNAL(clicked()),m,SLOT(map()));
    connect(ui->btn_point,SIGNAL(clicked()),m,SLOT(map()));

    m->setMapping(ui->btn_0,0);
    m->setMapping(ui->btn_1,1);
    m->setMapping(ui->btn_2,2);
    m->setMapping(ui->btn_3,3);
    m->setMapping(ui->btn_4,4);
    m->setMapping(ui->btn_5,5);
    m->setMapping(ui->btn_6,6);
    m->setMapping(ui->btn_7,7);
    m->setMapping(ui->btn_8,8);
    m->setMapping(ui->btn_9,9);
    m->setMapping(ui->btn_del,-1);
    m->setMapping(ui->btn_point,10);

    connect(m,SIGNAL(mapped(int)),this,SLOT(keyPress(int)));
}

void FV_SetDlg::keyPress(int value)
{
    if(ui->lineEdit->hasSelectedText())
        ui->lineEdit->clear();

    QString str = ui->lineEdit->text();
    if(value == -1) // 删除一个字符
    {
        if(str.size() > 0)
            str = str.left(str.length() - 1);
    }
    else if(value == 10)
        str.append(".");
    else
        str.append(QString::number(value));

    ui->lineEdit->setText(str);
}

/**
   * 功　能：输入按键响应函数
   * 入口参数：
   * 返回值：无
   */
void FV_SetDlg::on_btn_enter_clicked()
{
    QDoubleSpinBox *spinBox=NULL;

    switch(m_editBtn)
    {
    case 1:
        spinBox = ui->curMin;
        break;
    case 11:
        spinBox = ui->curMax;
        break;
    }

    if(spinBox)
    {
        double temp = ui->lineEdit->text().toDouble();
        spinBox->setValue(temp);
        ui->okBtn->setEnabled(true);
    }
}



/**
  * 功　能：设置电流值检查
  * 入口参数：
  * 返回值：无
  */
bool FV_SetDlg::checkCurValue(void)
{
    QString title = tr("电流阀值设置错误");
    double curMin, curMax;

    curMin = ui->curMin->value();
    curMax = ui->curMax->value();
    if(curMin > curMax){
        QMessageBox::critical(this,title ,tr("\n输出单元电流阀值设置不合法，请重新输入！\n 最小值不能大于最大值\n"));
        return false;
    }
    if(curMax > 16){
        QMessageBox::critical(this, title,tr("\n输出电流最大值设置不合法，请重新输入！\n 最大值不能大于16.0A\n"));
        return false;
    }

    return true;
}

/**
  * 功　能：设置电流值
  * 入口参数：
  * 返回值：无
  */
void FV_SetDlg::setCurValue(void)
{
    if(checkCurValue() == true)
    {
        short  curMin = (short)(ui->curMin->value() * 10);
        short  curMax  = (short)(ui->curMax->value() * 10);
        if(m_bit) // 设置某一位
        {
            m_curMin[m_bit-1] = curMin;
            m_curMax[m_bit-1] = curMax;
        }
        else // 统一设置
        {
            for(int i=0; i<NPMV_OUT_COUNT; ++i)
            {
                m_curMin[i] = curMin;
                m_curMax[i] = curMax;
            }
        }

        this->accept();
    }
}

void FV_SetDlg::on_okBtn_clicked()
{
    setCurValue();
}

void FV_SetDlg::on_cancelBtn_clicked()
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
