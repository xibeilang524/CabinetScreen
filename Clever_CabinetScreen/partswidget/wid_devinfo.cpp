/**
  * 设备信息窗口
  *         Lzy       2016-6-1
  */
#include "wid_devinfo.h"

WID_DevInfo::WID_DevInfo(QWidget *parent) :
    QWidget(parent)
{
    this->resize(parent->size());
    createControl();
    layoutControl();
}


/**
  * 功　能：创建控件
  * 入口参数：
  * 返回值：
  */
void WID_DevInfo::createControl(void)
{
    lab = new QLabel(this);

    nameLab = new QLabel(tr("设备名称"),this);
    name = new QLabel("RPDU1",this);
    name->setAlignment(Qt::AlignRight);

    typeLab = new QLabel(tr("设备型号"),this);
    type = new QLabel("RPDU(D)",this);
    type->setAlignment(Qt::AlignRight);

    stateLab = new QLabel(tr("工作状态"),this);
    state = new QLabel("正常",this);
    state->setAlignment(Qt::AlignRight);

    ipLab = new QLabel(tr("设备IP:"),this);
    ip = new QLabel("---.---.-.---",this);
    ip->setAlignment(Qt::AlignRight);
}


/**
 * @brief 布局
 */
void WID_DevInfo::layoutControl(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(lab);
    vLayout->addWidget(nameLab);
    vLayout->addWidget(name);

    vLayout->addWidget(typeLab);
    vLayout->addWidget(type);

    vLayout->addWidget(stateLab);
    vLayout->addWidget(state);

    vLayout->addWidget(ipLab);
    vLayout->addWidget(ip);
}

/**
  * 功　能：窗口信息更新
  * 入口参数：
  * 出口参数：
  * 返回值：
  */
void WID_DevInfo::updateWidget(void)
{
    QString str;

    if(m_devInfo.IP.size() == 4)
    {
        str.clear();
        for(int i=3; i>=0; i--)
            str +=  QString::number(m_devInfo.IP.at(i))+".";
        str = str.left(str.length() - 1);

        ip->setText(str);
    }
}


void WID_DevInfo::timeoutDone(void)
{
    if(m_devInfo.IP.size() > 0)
        updateWidget();
}

/**
  * 功　能：更新数据
  * 入口参数：data -> 设备结构体
  * 出口参数：
  * 返回值：
  */
void WID_DevInfo::updateData(NPM_sDevInfo &data)
{
    m_devInfo = data;
    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}
