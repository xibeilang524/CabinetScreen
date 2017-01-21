/**
  *  输出位的TableWidget
  *         Lzy       2016-6-1
  */
#include "op_tablewidget.h"

OP_TableWidget::OP_TableWidget(QWidget *parent) :
    QWidget(parent)
{
    m_tableWidget = new QTableWidget(this);
    layout = new QGridLayout(parent);
    layout->addWidget(m_tableWidget);

    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_rows = m_base = 0;
}


/**
 * @brief 初始化窗口
 */
void OP_TableWidget::initWidget(void)
{
    m_tableWidget->clear();
    m_tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header << tr("电流(A)") << tr("功率(KW)")<<tr("电能(Kwh)");
    m_tableWidget->setColumnCount(header.size());    //设置列数
    m_tableWidget->setHorizontalHeaderLabels(header);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void OP_TableWidget::addItemContent(int row, int column, QString &content)
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
void OP_TableWidget::addRowContent(QStringList &list)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
    list.clear();
}

/**
  * 功　能：初始化表
  * 入口参数：len -> 表示表行数m_tableWidget->horizontalHeader()->setVisible(false);
  * 返回值：
  */
void OP_TableWidget::initTable(int base,int rows)
{
    QStringList list, header;
    initWidget();

     m_rows = rows;
    for(int i=0; i<rows; ++i)
    {
        header << QString::number(base+i+1);
        for(int j=0; j<m_tableWidget->columnCount(); ++j)
            list << "---";
        addRowContent(list);
    }
    m_tableWidget->setVerticalHeaderLabels(header);
    m_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//自适应宽度
    m_tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
}


void OP_TableWidget::checkAlarm(int bit, QTableWidgetItem *item)
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


void OP_TableWidget::updateWidget(void)
{
    QString str;
    float temp = 0; int k=0;

    int vol = npm_get_pduData()->totalData.Tvol.first(); // 获取数据
    for(int i=0; i<m_rows; ++i)
    {
        temp = m_data.cur.at(m_base + i) / 10.0; // 电流值
        str = QString::number(temp,'f', 1);
        m_itemVec.at(k++)->setText(str);

        checkAlarm(m_base + i, m_itemVec.at(k-1));

        temp = m_data.cur.at(m_base + i) * vol / 10000.0; //功率值
        str = QString::number(temp,'f', 1);
        m_itemVec.at(k++)->setText(str);

        temp = m_data.kwh.at(m_base + i) / 10.0; // 电能
        str = QString::number(temp);
        m_itemVec.at(k++)->setText(str);
    }
}

void OP_TableWidget::timeoutDone(void)
{
    if((m_alarmflag.size() > 0) &&(m_data.swicth.size() > 0))
        updateWidget();
}


/**
  * 功　能：更新数据
  * 入口参数：
  * 返回值：
  */
void OP_TableWidget::updateData(int base,NPM_sCurKwh &data, QVector <short> &alarmflag)
{
    m_base = base;
    m_data = data;
    m_alarmflag = alarmflag;

    QTimer::singleShot(50,this,SLOT(timeoutDone()));
}
