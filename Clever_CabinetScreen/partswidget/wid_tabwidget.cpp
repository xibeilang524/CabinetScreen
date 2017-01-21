/**
  *   tab窗口
  *         Lzy       2016-6-1
  */
#include "wid_tabwidget.h"

WID_TabWIdget::WID_TabWIdget(QWidget *parent) :
    QWidget(parent)
{
    this->resize(parent->size());
    initWidget();
    initLayout();
}


/**
 * @brief 更新
 */
void WID_TabWIdget::updateWidget(NPM_sPDUData &pduData)
{
    m_output->updateData(pduData.curKwh, pduData.alarm.alarmflag);

    m_sensor->updateData(pduData.alarm);
}


/**
 * @brief 布局
 */
void WID_TabWIdget::initLayout(void)
{
    /*QGroupBox *groupBox = new QGroupBox(tr("状态信息"),this);

    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(m_tabWidget);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(groupBox);
    */

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(m_tabWidget);

}


void WID_TabWIdget::initWidget(void)
{
    m_tabWidget = new QTabWidget(this);

    m_output = new WID_OutputBit(m_tabWidget);
    m_tabWidget->addTab(m_output, tr("输出位状态"));

    m_sensor = new WID_Sensor(m_tabWidget);
    m_tabWidget->addTab(m_sensor, tr("传感器状态"));
}
