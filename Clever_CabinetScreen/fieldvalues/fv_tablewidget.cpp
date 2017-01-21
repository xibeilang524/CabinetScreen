/**
  * 阀值显示表格
  *         Lzy       2016-6-2
  */
#include "fv_tablewidget.h"

FV_TableWidget::FV_TableWidget(QWidget *parent) :
    QWidget(parent)
{
    m_tableWidget = new QTableWidget(this);
    layout = new QGridLayout(parent);
    layout->addWidget(m_tableWidget);

    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_rows = m_base = 0;
    m_oneTime = false;
}

FV_TableWidget::~FV_TableWidget()
{
    delete m_tableWidget;
}

/**
  * 功　能：初始化编辑按键
  * 入口参数：rows -> 行数    base --> 表示按键识别号基数
  * 返回值：
  */
void FV_TableWidget::initEditBtn(int base,int rows)
{
    m = new QSignalMapper(this);
    for(int i=0; i<rows; ++i)
    {
        QPushButton *btn = new QPushButton(tr("修改"),this);
        btn->setMaximumSize(80,60);

        connect(btn,SIGNAL(clicked()),m,SLOT(map()));
        m->setMapping(btn,base+i+1);

        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(btn);
         hLayout->setMargin(0);                          // 必须添加, 否则CheckBox不能正常显示
        hLayout->setAlignment(btn, Qt::AlignCenter);

        QWidget *widget = new QWidget(m_tableWidget);
        widget->setLayout(hLayout);

        m_tableWidget->setCellWidget(i,3,widget);
    }

    connect(m,SIGNAL(mapped(int)),this,SLOT(editBtnPress(int)));
}

/**
  * 功　能：编辑按键响应函数
  * 入口参数：value -> 按键值
  * 返回值：
  */
void FV_TableWidget::editBtnPress(int value)
{
    if( m_data.curMax.size() > 0)
        emit editBtnSig(value);
}

/**
  * 功　能：更新阀值
  * 入口参数：bit-> 输出位     0表示统一修改
  ×                     curMin -> 电流最小值
  ×                     curMax -> 电流最大值
  * 返回值：
  */
void FV_TableWidget::updateThreshold(int bit, short curMin, short curMax)
{
    QString strMin = QString::number(curMin/ 10.0,'f', 1);
    QString strMax = QString::number(curMax/ 10.0,'f', 1);
    int k=0;

    if(bit == 0)  // 统一修改阀值
    {
        for(int i=0; i<m_rows; ++i)
        {
            k++;
            m_itemVec.at(k++)->setText(strMin);
            m_itemVec.at(k++)->setText(strMax);
        }
    }
    else
    {
        k = (bit-1) *3 + 1;
        m_itemVec.at(k++)->setText(strMin);
        m_itemVec.at(k++)->setText(strMax);
    }
}


/**
 * @brief 初始化窗口
 */
void FV_TableWidget::initWidget(void)
{
    m_tableWidget->clear();
    m_tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header <<  tr("电流(A)") << tr("最小值(A)")<<tr("最大值(A)") << tr("编辑");
    m_tableWidget->setColumnCount(header.size());    //设置列数
    m_tableWidget->setHorizontalHeaderLabels(header);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void FV_TableWidget::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignCenter);
    m_tableWidget->setItem(row, column, item);

    m_itemVec.append(item); // 保存地址
}


/**
 * @brief 增加一行Content
 * @param list
 */
void FV_TableWidget::addRowContent(QStringList &list)
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
void FV_TableWidget::initTable(int base,int rows)
{
    QStringList list, header;
    initWidget();

     m_rows = rows;     
     m_base = base;

    for(int i=0; i<rows; ++i)
    {
        header << QString::number(base+i+1);
        for(int j=1; j<m_tableWidget->columnCount(); ++j)
            list << "---";
        addRowContent(list);
    }
    m_tableWidget->setVerticalHeaderLabels(header);
    m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//自适应宽度
    m_tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);

    //将行和列的大小设为与内容相匹配
     //m_tableWidget->resizeColumnsToContents();
   // m_tableWidget->resizeRowsToContents();

    initEditBtn(m_base, m_rows);
}



void FV_TableWidget::checkAlarm(int bit, QTableWidgetItem *item)
{
    extern bool alarm_output_bit(int bit);

    bool ret = alarm_output_bit(bit);
    if(ret)
    {
        QColor color(255,0,0);
         item->setTextColor(color);
    }
    else
    {
        QColor color(0,0,0);
         item->setTextColor(color);
    }
}


/**
  * 功　能：更新数据窗口
  * 入口参数：
  * 返回值：
  */
void FV_TableWidget::updateWidget(void)
{
    QString str;
    float temp = 0; int k=0;
    for(int i=0; i<m_rows; ++i)
    {
        temp = m_data.cur.at(m_base + i) / 10.0; // 电流值
        str = QString::number(temp,'f', 1);
        m_itemVec.at(k++)->setText(str);

        checkAlarm(m_base + i, m_itemVec.at(k-1));

        if(m_oneTime == false) // 只更新一次阀值
        {
            temp = m_data.curMin.at(m_base + i) / 10.0; // 最大值
            str = QString::number(temp,'f', 1);
            m_itemVec.at(k++)->setText(str);

            temp = m_data.curMax.at(m_base + i) / 10.0; // 最小值
            str = QString::number(temp,'f', 1);
            m_itemVec.at(k++)->setText(str);

        }
        else
            k += 2;
    }

     if(m_oneTime == false)
        m_oneTime = true;
}


void FV_TableWidget::timeoutDone(void)
{
    if(m_data.curMax.size() > 0)
        updateWidget();
}

 void FV_TableWidget::setUpdateFlag(void)
 {
     m_oneTime = false;
 }

/**
  * 功　能：更新数据
  * 入口参数：
  * 返回值：
  */
void FV_TableWidget::updateData(NPM_sCurKwh &data)
{
    m_data = data;

    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}
