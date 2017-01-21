/**
  * 传感器阀值tableWidget
  *         Lzy       2016-6-1
  */
#include "env_tablewidget.h"

ENV_TableWidget::ENV_TableWidget(QWidget *parent) :
    QWidget(parent)
{
    m_tableWidget = new QTableWidget(this);
    layout = new QGridLayout(parent);
    layout->addWidget(m_tableWidget);

    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    //m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_oneTime = false;
    initTable();
}


/**
  * 功　能：初始化编辑按键
  * 入口参数：rows -> 行数    base --> 表示按键识别号基数
  * 返回值：
  */
void ENV_TableWidget::initEditBtn(int base,int value)
{
    m = new QSignalMapper(this);
    for(int i=0; i<NPMV_TH_COUNT; ++i)
    {
        QPushButton *btn = new QPushButton(tr("修改"),this);

        connect(btn,SIGNAL(clicked()),m,SLOT(map()));
        m->setMapping(btn,value+i);

        m_tableWidget->setCellWidget(base+i,4,btn);
    }

    connect(m,SIGNAL(mapped(int)),this,SLOT(editBtnPress(int)));
}

/**
  * 功　能：编辑按键响应函数
  * 入口参数：value -> 按键值
  * 返回值：
  */
void ENV_TableWidget::editBtnPress(int value)
{
    emit editBtnSig(value);
}

/**
  * 功　能：温度设置
  * 入口参数：bit -> 修改位置 0 表示统一修改
                         min -> 最小值     max -> 最大值
  * 返回值：
  */
void ENV_TableWidget::setTemp(int bit, short min, short max)
{
    QString strMin = QString::number(min);
    QString strMax = QString::number(max);
    int k=0;
    if(bit == 0)
    {
        for(int i=0; i<NPMV_TH_COUNT; ++i)
        {
            k += 2;
            m_itemVec.at(k++)->setText(strMin);
            m_itemVec.at(k++)->setText(strMax);
        }
    }
    else
    {
        k = (bit-1) *4 + 2;
        m_itemVec.at(k++)->setText(strMin);
        m_itemVec.at(k++)->setText(strMax);
    }
}

/**
  * 功　能：湿度阀值设置
  * 入口参数：bit -> 修改位置 0 表示统一修改
                         min -> 最小值     max -> 最大值
  * 返回值：
  */
void ENV_TableWidget::setHum(int bit, short min, short max)
{
    QString strMin = QString::number(min);
    QString strMax = QString::number(max);
    int k=16;

    if(bit == 10)
    {
        for(int i=0; i<NPMV_TH_COUNT; ++i)
        {
            k += 2;
            m_itemVec.at(k++)->setText(strMin);
            m_itemVec.at(k++)->setText(strMax);
        }
    }
    else
    {
        bit -= 6;
        k = (bit-1) *4 + 2;
        m_itemVec.at(k++)->setText(strMin);
        m_itemVec.at(k++)->setText(strMax);
    }
}


/**
  * 功　能：更新阀值
  * 入口参数：bit-> 输出位     0表示统一修改
  ×                     min -> 电流最小值
  ×                     max -> 电流最大值
  * 返回值：
  */
void ENV_TableWidget::updateThreshold(int bit, short min, short max)
{
    if(bit<10)
        setTemp(bit,min,max);
    else
        setHum(bit,min,max);
}


/**
 * @brief 初始化窗口
 */
void ENV_TableWidget::initWidget(void)
{
    m_tableWidget->clear();
    m_tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header <<  tr("传感器名称") << tr("当前值") << tr("最小值")<<tr("最大值") << tr("编辑");
    m_tableWidget->setColumnCount(header.size());    //设置列数
    m_tableWidget->setHorizontalHeaderLabels(header);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void ENV_TableWidget::addItemContent(int row, int column, QString &content)
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
void ENV_TableWidget::addRowContent(QStringList &list)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
    list.clear();
}

/**
  * 功　能：初始化表
  * 入口参数：base -> 表示起始显示的基数
  *                     rows -> 表示要显示的总行数
  * 返回值：
  */
void ENV_TableWidget::initTable(void)
{
    QStringList list;
    initWidget();

    for(int i=0; i<NPMV_TH_COUNT; ++i)
    {
        list << tr("温度%1").arg(i+1);
        for(int j=2; j<m_tableWidget->columnCount(); ++j)
            list << "---";
        addRowContent(list);
    }
    initEditBtn(0,1);

    for(int i=0; i<NPMV_TH_COUNT; ++i)
    {
        list << tr("湿度%1").arg(i+1);
        for(int j=2; j<m_tableWidget->columnCount(); ++j)
            list << "---";
        addRowContent(list);
    }
    initEditBtn(4,11);

    m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//自适应宽度
    m_tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
}



/**
  * 功　能：更新数据窗口
  * 入口参数：
  * 返回值：
  */
void ENV_TableWidget::updateWidget(void)
{
    QString str;
    short temp = 0, k=0;

    for(int i=0; i<NPMV_TH_COUNT; ++i)
    {
        k++;
        temp = m_data.temp.at(i); // 温度值
        str = QString::number(temp);
        m_itemVec.at(k++)->setText(str);

        if(m_oneTime == false) // 只更新一次阀值
        {
            temp = m_data.tempMin.at( i); // 最小值
            str = QString::number(temp);
            m_itemVec.at(k++)->setText(str);

            temp = m_data.tempMax.at(i) ; // 最大值
            str = QString::number(temp);
            m_itemVec.at(k++)->setText(str);

            //      initEditBtn(m_base, m_rows);
        }
        else
            k += 2;
    }

    for(int i=0; i<NPMV_TH_COUNT; ++i)
    {
        k++;
        temp = m_data.hum.at(i); // 温度值
        str = QString::number(temp);
        m_itemVec.at(k++)->setText(str);

        if(m_oneTime == false) // 只更新一次阀值
        {
            temp = m_data.humMin.at( i); // 最小值
            str = QString::number(temp);
            m_itemVec.at(k++)->setText(str);

            temp = m_data.humMax.at(i) ; // 最大值
            str = QString::number(temp);
            m_itemVec.at(k++)->setText(str);

        }
        else
            k += 2;
    }

    if(m_oneTime == false)
        m_oneTime = true;
}


void ENV_TableWidget::timeoutDone(void)
{
    if(m_data.tempMax.size() > 0)
        updateWidget();
}


/**
  * 功　能：设置刷新标志
  * 入口参数：
  * 返回值：
  */
void ENV_TableWidget::setUpdateFlag(void)
{
    m_oneTime = false;
}

/**
  * 功　能：更新数据
  * 入口参数：
  * 返回值：
  */
void ENV_TableWidget::updateData(NPM_sAlarm &data)
{
    m_data = data;

    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}

