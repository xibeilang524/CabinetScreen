/*
 * log_tableview.cpp
 *
 *  Created on: 2015年10月11日
 *      Author: Lzy
 */
#include "log_tableview.h"

LOG_TableView::LOG_TableView(QWidget *parent) : QWidget(parent)
{
    view = new QTableView(this); //创建QTableView对象作为视图
    layout = new QGridLayout(parent);
    layout->addWidget(view,0,0);

    view->setSortingEnabled(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);//
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->resizeColumnsToContents();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁用编辑功能

    model = new DB_SqlTableModel(this);
    view->setModel(model->model);

    //    QHeaderView *header = view->horizontalHeader();
    //    header->setStretchLastSection(true);

    connect(view,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));
}

LOG_TableView::~LOG_TableView()
{

}

/**
 * @brief 初始化纪录日志
 */
void LOG_TableView::initRecordTable(void)
{
    m_table = LOG_RECORD_TABLE;
    this->refreshTable(m_table);

    QStringList head;
    head << "编号" << tr("日期") << tr("时间")  << tr("类型") << tr("描述");
    model->setHeaders(head);

    view->setColumnWidth(3,160);
    view->setColumnWidth(4,395);
}


/**
 * @brief 刷新表
 * @param table
 * @return
 */
bool LOG_TableView::refreshTable(QString &table)
{
    bool ret = model->refreshTable(table);
    if(ret)
    {
        m_table = table;
        view->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        view->setColumnHidden(0, true);
    }
    return  ret;
}

/**
 * @brief 双击删除
 */
void LOG_TableView::doubleSlot(QModelIndex)
{    
    delBtnSlot();
}

void LOG_TableView::refreshSlot(void)
{
    if(LOG_RECORD_TABLE == m_table)
        initRecordTable();
}

int LOG_TableView::getMsgBox(QString &str)
{
    QMessageBox msgBox(this);
    com_setBackColour(tr("信息提示"),&msgBox);
    msgBox.setText(str);
    msgBox.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    msgBox.setButtonText (QMessageBox::Ok,QString("确定"));
    msgBox.setButtonText (QMessageBox::Cancel,QString("取 消"));
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    return ret;
}

void LOG_TableView::clearBtnSlot(void)
{
    QString str = tr("\n确定清空日志信息\n");
    int ret = getMsgBox(str);
    if(ret ==  QMessageBox::Ok)
    {
        DB_SqlQuery sql;

        if(LOG_RECORD_TABLE == m_table)
            sql.delRecordTable();

        refreshSlot();
    }
}

void LOG_TableView::delBtnSlot(void)
{
    int curRow = view->currentIndex().row();
    if(curRow >= 0)
    {
        QString str = tr("\n是否删除这条纪录?\n");
        int ret = getMsgBox(str);
        if(ret ==  QMessageBox::Ok)
            model->removeRow(curRow);
    }
}
