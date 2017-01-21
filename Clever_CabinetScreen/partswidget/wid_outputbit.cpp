/**
  *   输出位状态显示
  *         Lzy       2016-6-1
  */
#include "wid_outputbit.h"

WID_OutputBit::WID_OutputBit(QWidget *parent) :
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
void WID_OutputBit::initWidget(void)
{
    m_tableWidget->clear();
    m_tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header << tr("输出位名称") << tr("开关" ) << tr("电流(A)") << tr("电能(Kwh)") << tr("临界");
    m_tableWidget->setColumnCount(header.size());    //设置列数
    m_tableWidget->setHorizontalHeaderLabels(header);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void WID_OutputBit::addItemContent(int row, int column, QString &content)
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
void WID_OutputBit::addRowContent(QStringList &list)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
    list.clear();
}

/**
  *
  */
void WID_OutputBit::initTable(void)
{
    QStringList list;
    int len = NPMV_OUT_COUNT;
    initWidget();

    for(int i=0; i<len; ++i)
    {
        list << "Output" +  QString::number(i+1, 10);  // 显示输出位
        for(int j=1; j<m_tableWidget->columnCount(); ++j)
            list << "---";
        addRowContent(list);
    }

   // m_tableWidget->horizontalHeader()->setStretchLastSection(true); //自适应宽度
    //将行和列的大小设为与内容相匹配
    m_tableWidget->resizeColumnsToContents();
    m_tableWidget->resizeRowsToContents();
}

void WID_OutputBit::updateWidget(void)
{
    QString str;
    float temp = 0; int k=0;
    for(int i=0; i<NPMV_OUT_COUNT; ++i)
    {
        k++;
        if(m_data.swicth.at(i)) // 输出位状态
            str = tr("开");
        else
            str = tr("关");
        m_itemVec.at(k++)->setText(str);

        temp = m_data.cur.at(i) / 10.0; // 电流值
        str = QString::number(temp);
        m_itemVec.at(k++)->setText(str);

        temp = m_data.kwh.at(i) / 10.0; // 电能
        str = QString::number(temp);
        m_itemVec.at(k++)->setText(str);

        if(m_alarmflag.at(i))
            str = tr("异常");
        else
            str = tr("正常");
        m_itemVec.at(k++)->setText(str);
    }
}

void WID_OutputBit::timeoutDone(void)
{
    if(m_alarmflag.size() > 0)
        updateWidget();
}


/**
  * 功　能：更新数据
  * 入口参数：
  * 返回值：
  */
void WID_OutputBit::updateData(NPM_sCurKwh &data, QVector <short> &alarmflag)
{
    m_data = data;
    m_alarmflag = alarmflag;

    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}


