/**
  *  修改总电流阀值
  *         Lzy       2016-6-1
  */
#include "total_thresholdset.h"
#include "ui_total_thresholdset.h"

TOTAL_ThresholdSet::TOTAL_ThresholdSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TOTAL_ThresholdSet)
{
    ui->setupUi(this);
    com_setBackColour(tr("总电流阀值设置"),this);

    m_editBtn = 0;
    ui->groupBox->setHidden(true);

    initKey();
    initEditBtn();

    ui->okBtn->setEnabled(false);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

TOTAL_ThresholdSet::~TOTAL_ThresholdSet()
{
    delete ui;
}

void TOTAL_ThresholdSet::setData(QVector <short> &min, QVector <short> &max)
{
    m_tcurMin = min;
    m_tcurMax = max;

    QTimer::singleShot(5,this,SLOT(InitFunction()));
}

 void TOTAL_ThresholdSet::getData(QVector <short> &min, QVector <short> &max)
 {
     min = m_tcurMin;
     max = m_tcurMax;
 }


/**
  * 功　能：设置电流值检查
  * 入口参数：
  * 返回值：无
  */
bool TOTAL_ThresholdSet::checkCurValue(void)
{
    QString title = tr("总电流阀值设置错误");
    double curMin, curMax;

    curMin = ui->curMin_1->value();
    curMax = ui->curMax_1->value();
    if(curMin > curMax){
        QMessageBox::critical(this,title ,tr("\n总电流阀值设置不合法，请重新输入！\n 最小值不能大于最大值\n"));
        return false;
    }
    if(curMax > 32){
        QMessageBox::critical(this, title,tr("\n总电流最大值设置不合法，请重新输入！\n 最大值不能大于32.0A\n"));
        return false;
    }
    return true;// 检查单相

    curMin = ui->curMin_2->value();
    curMax = ui->curMax_2->value();
    if(curMin > curMax){
        QMessageBox::critical(this,title ,tr("相数(L2)电流阀值设置不合法，请重新输入！\n 最小值不能大于最大值"));
        return false;
    }
    if(curMax > 32){
        QMessageBox::critical(this, title,tr("相数(L2)电流最大值设置不合法，请重新输入！\n 最大值不能大于32.0A"));
        return false;
    }

    curMin = ui->curMin_3->value();
    curMax = ui->curMax_3->value();
    if(curMin > curMax){
        QMessageBox::critical(this,title ,tr("相数(L3)电流阀值设置不合法，请重新输入！\n 最小值不能大于最大值"));
        return false;
    }
    if(curMax > 32){
        QMessageBox::critical(this, title,tr("相数(L3)电流最大值设置不合法，请重新输入！\n 最大值不能大于32.0A"));
        return false;
    }

    return true;
}

/**
  * 功　能：设置电流值
  * 入口参数：
  * 返回值：无
  */
void TOTAL_ThresholdSet::setCurValue(void)
{
    if(checkCurValue() == true)
    {
        m_tcurMin.clear();
        short temp = (short)(ui->curMin_1->value() * 10);
        m_tcurMin.append(temp);
        temp = (short)(ui->curMin_2->value() * 10);
        m_tcurMin.append(temp);
        temp = (short)(ui->curMin_3->value() * 10);
        m_tcurMin.append(temp);

        m_tcurMax.clear();
        temp = (short)(ui->curMax_1->value() * 10);
        m_tcurMax.append(temp);
        temp = (short)(ui->curMax_2->value() * 10);
        m_tcurMax.append(temp);
        temp = (short)(ui->curMax_3->value() * 10);
        m_tcurMax.append(temp);

        emit setThresholdSig();
        this->accept();
    }
}

/**
  * 功　能：显示阀值
  * 入口参数：
  * 返回值：无
  */
void TOTAL_ThresholdSet::initWidget(void)
{
    float temp = m_tcurMin.at(0)/10.0;
    ui->curMin_1->setValue(temp);

    temp = m_tcurMin.at(1)/10.0;
    ui->curMin_2->setValue(temp);

    temp = m_tcurMin.at(2)/10.0;
    ui->curMin_3->setValue(temp);

    temp = m_tcurMax.at(0)/10.0;
    ui->curMax_1->setValue(temp);

    temp = m_tcurMax.at(1)/10.0;
    ui->curMax_2->setValue(temp);

    temp = m_tcurMax.at(2)/10.0;
    ui->curMax_3->setValue(temp);
}

/**
  * 功　能：初始化功能函数
  * 入口参数：
  * 返回值：无
  */
void TOTAL_ThresholdSet::InitFunction(void)
{
    initWidget();

    timeoutDone();
    timer->start(1000);
}


void TOTAL_ThresholdSet::checkAlarm(void)
{
    extern bool alarm_line_cur(int line);

    bool ret = alarm_line_cur(0);
    if(ret)
        ui->curEdit_1->setStyleSheet("color: rgb(255, 0, 0);");
    else
        ui->curEdit_1->setStyleSheet("color:black;");
}


/**
   * 功　能：更新当前电流值
   * 入口参数：
   * 返回值：无
   */
void TOTAL_ThresholdSet::updateData(void)
{
    float temp = m_totalData.Tcur.at(0)/10.0;
    QString str = QString::number(temp,'f', 1);
    ui->curEdit_1->setText(str);

    temp = m_totalData.Tcur.at(1)/10.0;
    str = QString::number(temp,'f', 1);
    ui->curEdit_2->setText(str);

    temp = m_totalData.Tcur.at(2)/10.0;
    str = QString::number(temp,'f', 1);
    ui->curEdit_3->setText(str);
}

/**
   * 功　能：定时器响应函数
   * 入口参数：
   * 返回值：无
   */
void TOTAL_ThresholdSet::timeoutDone(void)
{
    m_totalData = npm_get_pduData()->totalData; // 获取数据

    if(m_totalData.TcurMax.size() > 0)  // 已经收取到数据
    {
        updateData();
        checkAlarm();
    }
}

/**
   * 功　能：初始化编辑按键
   * 入口参数：
   * 返回值：无
   */
void TOTAL_ThresholdSet::initEditBtn(void)
{
    sm = new QSignalMapper(this);
    connect(ui->curMinBtn_1,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->curMinBtn_2,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->curMinBtn_3,SIGNAL(clicked()),sm,SLOT(map()));

    connect(ui->curMaxBtn_1,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->curMaxBtn_2,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->curMaxBtn_3,SIGNAL(clicked()),sm,SLOT(map()));

    sm->setMapping(ui->curMinBtn_1,1);
    sm->setMapping(ui->curMinBtn_2,2);
    sm->setMapping(ui->curMinBtn_3,3);

    sm->setMapping(ui->curMaxBtn_1,11);
    sm->setMapping(ui->curMaxBtn_2,12);
    sm->setMapping(ui->curMaxBtn_3,13);

    connect(sm,SIGNAL(mapped(int)),this,SLOT(editBtnPress(int)));
}

/**
   * 功　能：初始化背景色
   * 入口参数：
   * 返回值：无
   */
void TOTAL_ThresholdSet::initBackground(void)
{
    QString str =  "background-color:white;";

    ui->curMin_1->setStyleSheet(str);
    ui->curMin_2->setStyleSheet(str);
    ui->curMin_3->setStyleSheet(str);

    ui->curMax_1->setStyleSheet(str);
    ui->curMax_2->setStyleSheet(str);
    ui->curMax_3->setStyleSheet(str);
}

void TOTAL_ThresholdSet::editBtnPress(int value)
{
    QDoubleSpinBox *spinBox=NULL;
    QString colour = "background-color:rgb(199,237,204)";

    m_editBtn = value;
    switch(value)
    {
    case 1:
        spinBox = ui->curMin_1;
        break;
    case 2:
        spinBox = ui->curMin_2;
        break;
    case 3:
        spinBox = ui->curMin_3;
        break;

    case 11:
        spinBox = ui->curMax_1;
        break;
    case 12:
        spinBox = ui->curMax_2;
        break;
    case 13:
        spinBox = ui->curMax_3;
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
void TOTAL_ThresholdSet::initKey(void)
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

void TOTAL_ThresholdSet::keyPress(int value)
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
void TOTAL_ThresholdSet::on_btn_enter_clicked()
{
    QDoubleSpinBox *spinBox=NULL;

    switch(m_editBtn)
    {
    case 1:
        spinBox = ui->curMin_1;
        break;
    case 2:
        spinBox = ui->curMin_2;
        break;
    case 3:
        spinBox = ui->curMin_3;
        break;

    case 11:
        spinBox = ui->curMax_1;
        break;
    case 12:
        spinBox = ui->curMax_2;
        break;
    case 13:
        spinBox = ui->curMax_3;
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
   * 功　能：确定按键
   * 入口参数：
   * 返回值：无
   */
void TOTAL_ThresholdSet::on_okBtn_clicked()
{
    setCurValue();
}

void TOTAL_ThresholdSet::on_cancelBtn_clicked()
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
