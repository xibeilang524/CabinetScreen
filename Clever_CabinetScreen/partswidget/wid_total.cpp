/**
  *   状态显示
  *         Lzy       2016-6-1
  */
#include "wid_total.h"

WID_Total::WID_Total(QWidget *parent) :
    QWidget(parent)
{
    this->resize(parent->size());
    createControl();
    layoutControl();

    m_line = 0;
}

/**
  * 功　能：创建控件
  * 入口参数：
  * 返回值：
  */
void WID_Total::createControl(void)
{
    QFont ft;
    ft.setPointSize(12);

    lab = new QLabel(this);

    curLab = new QLabel(tr("总电流："),this);
    //curLab->setAlignment(Qt::AlignCenter);
    curLab->setMaximumHeight(40);
    curLab->setFont(ft);

    cur = new QLabel("---",this);
    cur->setAlignment(Qt::AlignRight);
    //cur->setMaximumHeight(40);
    cur->setFont(ft);

    volLab = new QLabel(tr("总电压："),this);
    //volLab->setAlignment(Qt::AlignCenter);
    volLab->setMaximumHeight(40);
    volLab->setFont(ft);

    vol = new QLabel("---",this);
    vol->setAlignment(Qt::AlignRight);
    //vol->setMaximumHeight(40);
    vol->setFont(ft);

    tpfLab = new QLabel(tr("功率因素"),this);
   // tpfLab->setAlignment(Qt::AlignCenter);
    tpfLab->setMaximumHeight(40);
    //tpfLab->setFont(ft);

    tpf = new QLabel("---",this);
    tpf->setAlignment(Qt::AlignRight);
    //tpf->setMaximumHeight(40);
    tpf->setFont(ft);

    kwhLab = new QLabel(tr("总电能："),this);
   // kwhLab->setAlignment(Qt::AlignCenter);
    kwhLab->setMaximumHeight(40);
    kwhLab->setFont(ft);

    kwh = new QLabel("---",this);
    kwh->setAlignment(Qt::AlignRight);
    // kwh->setMaximumHeight(40);
    //kwh->setFont(ft);

    radio1 = new QRadioButton("Line 1", this);
    radio2 = new QRadioButton("Line 2", this);
    radio3 = new QRadioButton("Line 3", this);

    radio1->setChecked(true);

    connect(radio1, SIGNAL(clicked()), this, SLOT(onRadio1Clicked()));
    connect(radio2, SIGNAL(clicked()), this, SLOT(onRadio2Clicked()));
    connect(radio3, SIGNAL(clicked()), this, SLOT(onRadio3Clicked()));
}


/**
 * @brief 布局
 */
void WID_Total::layoutControl(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(lab);
    vLayout->addWidget(curLab);
    vLayout->addWidget(cur);

    vLayout->addWidget(volLab);
    vLayout->addWidget(vol);

    vLayout->addWidget(tpfLab);
    vLayout->addWidget(tpf);

    vLayout->addWidget(kwhLab);
    vLayout->addWidget(kwh);

    vLayout->addWidget(radio1);
    vLayout->addWidget(radio2);
    vLayout->addWidget(radio3);
}

/**
  * 功　能：更新显示
  * 返回值：
  */
void WID_Total::updateWidget(void)
{
    float temp = m_totalData.Tcur.at(m_line) / 10.0;
    QString str = QString::number(temp,'f', 1) + "A";
    cur->setText(str);

    temp = m_totalData.Tvol.at(m_line);
    str = QString::number(temp) +"V";
    vol->setText(str);

    temp = m_totalData.Tpf.at(m_line) /100.0;
    str = QString::number(temp,'f', 2);
    tpf->setText(str);

    temp = m_totalData.TKwh.at(m_line) /10.0;
    str = QString::number(temp,'f', 1)+"Kwh";
    kwh->setText(str);
}


void WID_Total::timeoutDone(void)
{
    if(m_totalData.TKwh.size() > 0)
        updateWidget();
}

/**
  * 功　能：更新数据
  * 入口参数：data -> 设备结构体
  * 出口参数：
  * 返回值：
  */
 void WID_Total::updateData(NPM_sTotalData &data)
 {
     m_totalData = data;
      QTimer::singleShot(50,this,SLOT(timeoutDone()));
 }

void WID_Total::onRadio1Clicked()
{
    m_line =0;
    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}

void WID_Total::onRadio2Clicked()
{
    m_line =1;
    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}

void WID_Total::onRadio3Clicked()
{
    m_line =2;
    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}
