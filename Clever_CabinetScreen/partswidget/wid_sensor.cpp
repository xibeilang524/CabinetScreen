/**
  *   传感器状态显示
  *         Lzy       2016-6-1
  */
#include "wid_sensor.h"


WID_Sensor::WID_Sensor(QWidget *parent) :
    QWidget(parent)
{
    m_tableWidget = new QTableWidget(this);
    layout = new QGridLayout(this);
    layout->addWidget(m_tableWidget);

    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    initTable();
}


/**
 * @brief 初始化窗口
 */
void WID_Sensor::initWidget(void)
{
    m_tableWidget->clear();
    m_tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header << tr("传感器名称") << tr("状态" ) << tr("传感器名称") << tr("状态" ) ;
    m_tableWidget->setColumnCount(header.size());    //设置列数
    m_tableWidget->setHorizontalHeaderLabels(header);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void WID_Sensor::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignHCenter);
    m_tableWidget->setItem(row, column, item);

    m_itemVec.append(item); // 保存地址
}


/**
 * @brief 增加一行Content
 * @param list
 */
void WID_Sensor::addRowContent(QStringList &list)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
    list.clear();
}

/**
  * 初始化表格
  */
void WID_Sensor::initTable(void)
{
    QStringList list;
    int len = NPMV_TH_COUNT;
    initWidget();

    for(int i=0; i<len; ++i)
    {
        list << "温度" +  QString::number(i+1, 10);
        list << tr("未启用");
        list << "湿度" +  QString::number(i+1, 10);
        list << tr("未启用");
        addRowContent(list);
    }

    list << tr("门禁1")  << tr("未启用") << tr("门禁2")  << tr("未启用");
    addRowContent(list);

    list << tr("水浸")  << tr("未启用") << tr("烟雾")  << tr("未启用");
    addRowContent(list);

    m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);  //自适应宽度
}


void WID_Sensor::updateWidget(void)
{
    QString str;
    int temp = 0, k=1;
    for(int i=0; i<NPMV_TH_COUNT; ++i)
    {
        temp = m_sensor.temp.at(i) ;
        if(temp)
            str = QString::number(temp) + "℃";
        else
            str = tr("未启用");
        m_itemVec.at(k++)->setText(str);
        k++;

        temp = m_sensor.hum.at(i);
        if(temp)
            str = QString::number(temp)  + "%";
            else
            str = tr("未启用");
        m_itemVec.at(k++)->setText(str);
           k++;
    }

    for(int i=0; i<NPMV_DOOR_COUNT; ++i)
    {
        temp = m_sensor.door.at(i);
        if(temp == 0)
            str = tr("未启用");
        else if(temp == 1)
            str = tr("打开");
        else if(temp == 2)
            str = tr("关闭");
        else
            str = tr("报警");
        m_itemVec.at(k++)->setText(str);
        k++;
    }

    temp= m_sensor.water.at(0);
    if(temp == 0)
        str = tr("未启用");
    else if(temp == 1)
        str = tr("正常");
    else
        str = tr("报警");
    m_itemVec.at(k++)->setText(str);
    k++;

    temp = m_sensor.smoke.at(0);
    if(temp == 0)
        str = tr("未启用");
    else if(temp == 1)
        str = tr("正常");
    else
        str = tr("报警");
    m_itemVec.at(k++)->setText(str);
}


void WID_Sensor::timeoutDone(void)
{
    if(m_sensor.water.size() > 0)
        updateWidget();
}


/**
  * 功　能：更新数据
  * 入口参数：
  * 返回值：
  */
void WID_Sensor::updateData(NPM_sAlarm &data)
{
    m_sensor = data;

    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}
