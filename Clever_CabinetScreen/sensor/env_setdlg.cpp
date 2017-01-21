/**
  * 传感器阀值修改窗口
  *         Lzy       2016-6-1
  */
#include "env_setdlg.h"
#include "ui_env_setdlg.h"
#include <QMessageBox>

ENV_SetDlg::ENV_SetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ENV_SetDlg)
{
    ui->setupUi(this);
    m_editBtn = 0;
    ui->okBtn->setEnabled(false);

    initEditBtn();
    initKey();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}

ENV_SetDlg::~ENV_SetDlg()
{
    delete ui;
}

/**
   * 功　能：初始化数据
   * 入口参数：QString -> 标题名
   * 返回值：无
   */
void ENV_SetDlg::initData(QString str, short min, short max)
{
    ui->label->setText(str);
    ui->minSpin->setValue(min);
    ui->maxSpin->setValue(max);

    if(str.size() < 3)
        ui->curEdit->setEnabled(false);
}

/**
   * 功　能：更新当前值
   * 入口参数：value -> 小于10表示温度  大于10表示湿度
   * 返回值：无
   */
void ENV_SetDlg::updateData(short value)
{
    short temp=0;
    m_bit = value;
    NPM_sPDUData *pduData = npm_get_pduData();

    if(value < 10) // 更新温度
    {
        if(value)
            value--;

        temp = pduData->alarm.temp.at(value);
    }
    else
    {
        value -= 10;
        if(value)
            value--;
        temp = pduData->alarm.hum.at(value);
    }

    if(temp)
        ui->curEdit->setText(QString::number(temp));
    else
        ui->curEdit->setText("未启用");
}


/**
   * 功　能：定时函数
   * 入口参数：
   * 返回值：无
   */
void ENV_SetDlg::timeoutDone(void)
{
    updateData(m_bit);
}


/**
   * 功　能：初始化编辑按键
   * 入口参数：
   * 返回值：无
   */
void ENV_SetDlg::initEditBtn(void)
{
    sm = new QSignalMapper(this);
    connect(ui->minBtn,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->maxBtn,SIGNAL(clicked()),sm,SLOT(map()));

    sm->setMapping(ui->minBtn,1);
    sm->setMapping(ui->maxBtn,11);

    connect(sm,SIGNAL(mapped(int)),this,SLOT(editBtnPress(int)));
}

/**
   * 功　能：初始化背景色
   * 入口参数：
   * 返回值：无
   */
void ENV_SetDlg::initBackground(void)
{
    QString str =  "background-color:white;";
    ui->minSpin->setStyleSheet(str);
    ui->maxSpin->setStyleSheet(str);
}

void ENV_SetDlg::editBtnPress(int value)
{
    QSpinBox *spinBox=NULL;
    QString colour = "background-color:rgb(199,237,204)";

    m_editBtn = value;
    switch(value)
    {
    case 1:
        spinBox = ui->minSpin;
        break;

    case 11:
        spinBox = ui->maxSpin;
        break;

    }
    initBackground();
    spinBox->setStyleSheet(colour);

    int temp = spinBox->value();
    QString str = QString::number(temp);
    ui->lineEdit->setText(str);
}

/**
   * 功　能：初始化键盘按键
   * 入口参数：
   * 返回值：无
   */
void ENV_SetDlg::initKey(void)
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

void ENV_SetDlg::keyPress(int value)
{
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
void ENV_SetDlg::on_btn_enter_clicked()
{
    QSpinBox *spinBox=NULL;

    switch(m_editBtn)
    {
    case 1:
        spinBox = ui->minSpin;
        break;
    case 11:
        spinBox = ui->maxSpin;
        break;
    }

    if(spinBox)
    {
        int temp = ui->lineEdit->text().toInt();
        spinBox->setValue(temp);
        ui->okBtn->setEnabled(true);
    }
}

/**
  * 功　能：设置电流值检查
  * 入口参数：
  * 返回值：无
  */
bool ENV_SetDlg::checkCurValue(void)
{
    QString title = tr("传感器阀值设置错误");

    m_min = ui->minSpin->value();
    m_max = ui->maxSpin->value();
    if(m_min > m_max){
        QMessageBox::critical(this,title ,tr("传感器阀值设置不合法，请重新输入！\n 最小值不能大于最大值"));
        return false;
    }
    if(m_max > 99){
        QMessageBox::critical(this, title,tr("最大值设置不合法，请重新输入！\n 最大值不能大于64.0A"));
        return false;
    }

    return true;
}

void ENV_SetDlg::on_okBtn_clicked()
{
    bool ret = checkCurValue();
    if(ret)
        this->accept();
}
