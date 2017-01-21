/**
  * 时间设置窗口
  *         Lzy       2016-6-2
  */
#include "timesettingdlg.h"
#include "ui_timesettingdlg.h"
#include <QMessageBox>
#include <QTextCharFormat>
extern void com_setBackColour(const QString &str,QWidget *target);

TimeSettingDlg::TimeSettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeSettingDlg)
{
    ui->setupUi(this);
    com_setBackColour(tr("时间设置"),this);

    QDate date = QDate::currentDate();
    ui->yearSpin->setValue(date.year());
    ui->monSpin->setValue(date.month());
    ui->daySpin->setValue(date.day());

    m_editBtn =0;
    ui->timeSet_but->setEnabled(false);

    ui->hourSpin->setValue(QDateTime::currentDateTime().time().hour());
    ui->minSpin->setValue(QDateTime::currentDateTime().time().minute());
    ui->secSpin->setValue(QDateTime::currentDateTime().time().second());

    preYear = ui->yearSpin->value();
    preMonth = ui->monSpin->value();

    connect(ui->monSpin, SIGNAL(valueChanged(int)), this, SLOT(dateSetLimit(int)));
    connect(ui->yearSpin, SIGNAL(valueChanged(int)), this, SLOT(dateSetLimit(int)));
    connect(ui->timeSet_but, SIGNAL(clicked()), this, SLOT(timeSetup()));

    initEditBtn();
    initKey();
}

TimeSettingDlg::~TimeSettingDlg()
{
    delete ui;
}



/**
 * 功 能：验证日期合法性
 */
void TimeSettingDlg::dateSetLimit(int)
{
    switch(ui->monSpin->value())
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (ui->daySpin->value() > 31)
            goto SHOWMESSAGEBOX;
        ui->daySpin->setMaximum(31);
        break;
    case 2:
        if (isLeap(ui->yearSpin->value())){
            if (ui->daySpin->value() > 29)
                goto SHOWMESSAGEBOX;
            ui->daySpin->setMaximum(29);
        }
        else{
            if (ui->daySpin->value() > 28)
                goto SHOWMESSAGEBOX;
            ui->daySpin->setMaximum(28);
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (ui->daySpin->value() > 30)
            goto SHOWMESSAGEBOX;
        ui->daySpin->setMaximum(30);
        break;
    }
    preYear = ui->yearSpin->value();
    preMonth = ui->monSpin->value();
    return;

SHOWMESSAGEBOX:
    ui->yearSpin->setValue(preYear);
    ui->monSpin->setValue(preMonth);

    QMessageBox::critical(this, tr("信息提示"),tr("日期设置不合法，请重新输入"));
}

/**
 * 功 能：润年
 */
bool TimeSettingDlg::isLeap(int yN)
{
    if ((yN % 400 == 0 && yN % 3200 != 0)
            || (yN % 4 == 0 && yN % 100 != 0)
            || (yN % 3200 == 0 && yN % 172800 == 0))
        return true;
    else
        return false;

}


/**
 * 功 能：时间设置
 */
void TimeSettingDlg::timeSetup(void)
{
    QDateTime currentTime_old = QDateTime::currentDateTime();
    int year_old = currentTime_old.date().year();
    int month_old = currentTime_old.date().month();
    int day_old = currentTime_old.date().day();
    int hour_old = currentTime_old.time().hour();
    int minute_old = currentTime_old.time().minute();

    struct tm setDataTime;
    time_t timep;
    struct timeval tv;

    setDataTime.tm_year = ui->yearSpin->value() - 1900;
    setDataTime.tm_mon = ui->monSpin->value() - 1;
    setDataTime.tm_mday = ui->daySpin->value();

    setDataTime.tm_hour = ui->hourSpin->value();
    setDataTime.tm_min = ui->minSpin->value();
    setDataTime.tm_sec = ui->secSpin->value();

    timep = mktime(&setDataTime);

    tv.tv_sec = timep;
    tv.tv_usec = 0;

    if (settimeofday(&tv, (struct timezone *)0) < 0)
        return;

    int ret = system("hwclock -w");
    if(ret >= 0)
    {
        QString str;
        QDateTime currentTime_new = QDateTime::currentDateTime();
        int year_new = currentTime_new.date().year();
        int month_new = currentTime_new.date().month();
        int day_new = currentTime_new.date().day();
        int hour_new = currentTime_new.time().hour();
        int minute_new = currentTime_new.time().minute();

        if (year_old != year_new || month_old != month_new
                || day_old != day_new || hour_old != hour_new
                || minute_old != minute_new)  {
            str = tr("时间设置成功");           
        }  else {
            str = tr("时间未修改");
            qDebug() << str;
        }
        QMessageBox::information(this,tr("信息提示"),str);
        ui->timeSet_but->setEnabled(false);
    }
}



/**
   * 功　能：初始化编辑按键
   * 入口参数：
   * 返回值：无
   */
void TimeSettingDlg::initEditBtn(void)
{
    sm = new QSignalMapper(this);
    connect(ui->hourBtn,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->minBtn,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->secBtn,SIGNAL(clicked()),sm,SLOT(map()));

    connect(ui->yearBtn,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->monBtn,SIGNAL(clicked()),sm,SLOT(map()));
    connect(ui->dayBtn,SIGNAL(clicked()),sm,SLOT(map()));

    sm->setMapping(ui->hourBtn,1);
    sm->setMapping(ui->minBtn,2);
    sm->setMapping(ui->secBtn,3);

    sm->setMapping(ui->yearBtn,4);
    sm->setMapping(ui->monBtn,5);
    sm->setMapping(ui->dayBtn,6);

    connect(sm,SIGNAL(mapped(int)),this,SLOT(editBtnPress(int)));
}

/**
   * 功　能：初始化背景色
   * 入口参数：
   * 返回值：无
  */
void TimeSettingDlg::initBackground(void)
{
    QString str =  "background-color:white;";

    ui->hourSpin->setStyleSheet(str);
    ui->minSpin->setStyleSheet(str);
    ui->secSpin->setStyleSheet(str);

    ui->yearSpin->setStyleSheet(str);
    ui->monSpin->setStyleSheet(str);
    ui->daySpin->setStyleSheet(str);
}

void TimeSettingDlg::editBtnPress(int value)
{
    QSpinBox *spinBox=NULL;
    QString colour = "background-color:rgb(199,237,204)";

    m_editBtn = value;
    switch(value)
    {
    case 1:
        spinBox = ui->hourSpin;
        break;
    case 2:
        spinBox = ui->minSpin;
        break;
    case 3:
        spinBox = ui->secSpin;
        break;

    case 4:
        spinBox = ui->yearSpin;
        break;
    case 5:
        spinBox = ui->monSpin;
        break;
    case 6:
        spinBox = ui->daySpin;
        break;
    }
    initBackground();
    spinBox->setStyleSheet(colour);

    int temp = spinBox->value();
    QString str = QString::number(temp);
    ui->lineEdit->setText(str);
    ui->lineEdit->selectAll();
}

/**
   * 功　能：初始化键盘按键
   * 入口参数：
   * 返回值：无
 */
void TimeSettingDlg::initKey(void)
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

void TimeSettingDlg::keyPress(int value)
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



void TimeSettingDlg::on_btn_enter_clicked()
{
    QSpinBox *spinBox=NULL;

    switch(m_editBtn)
    {
    case 1:
        spinBox = ui->hourSpin;
        break;
    case 2:
        spinBox = ui->minSpin;
        break;
    case 3:
        spinBox = ui->secSpin;
        break;

    case 4:
        spinBox = ui->yearSpin;
        break;
    case 5:
        spinBox = ui->monSpin;
        break;
    case 6:
        spinBox = ui->daySpin;
        break;
    }

    if(spinBox)
    {
        int temp = ui->lineEdit->text().toInt();
        spinBox->setValue(temp);
        ui->timeSet_but->setEnabled(true);
    }
}

void TimeSettingDlg::on_quitBtn_clicked()
{
    bool ret = ui->timeSet_but->isEnabled();
    if(ret)
    {
        QMessageBox msgBox(this);
         com_setBackColour(tr("信息提示"),&msgBox);
        msgBox.setText(tr("\n您已修改时间，是否要放弃修改？\n"));
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
