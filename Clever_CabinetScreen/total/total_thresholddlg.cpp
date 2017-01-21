#include "total_thresholddlg.h"
#include "ui_total_thresholddlg.h"
#include "total_waitdlg.h"

TOTAL_ThresholdDlg::TOTAL_ThresholdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TOTAL_ThresholdDlg)
{
    ui->setupUi(this);
     com_setBackColour(tr("总电流阀值"),this);
     //com_setBackColour("",ui->tableWidget);

    m_initFlag = false;
    m_totalData = npm_get_pduData()->totalData; // 获取数据
    QTimer::singleShot(5,this,SLOT(InitFunction()));

    initTableWidget();
    ui->saveBtn->setEnabled(false);
}

TOTAL_ThresholdDlg::~TOTAL_ThresholdDlg()
{
    delete ui;
}




/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void TOTAL_ThresholdDlg::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, column, item);

    m_itemVec.append(item); // 保存地址
}


/**
 * @brief 增加一行Content
 * @param list
 */
void TOTAL_ThresholdDlg::addRowContent(QStringList &list)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
    list.clear();
}


/**
  * 功　能：初始化tableWidget
    QVector <short> m_tcurMin; // 总电流下限
    QVector <short> m_tcurMax; // 总电流上限
  * 入口参数：
  * 返回值：无
  */
void TOTAL_ThresholdDlg::initTableWidget(void)
{
    QStringList header;
    header << tr("名称") << tr("当前值(A)" ) << tr("最小值(A)") << tr("最大值(A)" );
    ui->tableWidget->setColumnCount(header.size());    //设置列数
    ui->tableWidget->setHorizontalHeaderLabels(header);

    QStringList list;
    int len = NPMV_LINE_COUNT;
    for(int i=0; i<len; ++i)
    {
       // list << tr("总电流(L") +  QString::number(i+1, 10) +")";
        list << tr("总电流");
        if(m_totalData.TcurMax.size()>0)
        {
            float temp = m_totalData.Tcur.at(i) / 10.0;
            list << QString::number(temp,'f', 1);

            temp = m_totalData.TcurMin.at(i) / 10.0;
            list << QString::number(temp,'f', 1);

            temp = m_totalData.TcurMax.at(i) / 10.0;
            list << QString::number(temp,'f', 1);

        }
        else
            list << "---" <<"---" <<"---";

        addRowContent(list);

        break;      // 只显示第一相
    }

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);  //自适应宽度
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
}


void TOTAL_ThresholdDlg::checkAlarm(int line, QTableWidgetItem *item)
{
    extern bool alarm_line_cur(int line);

    bool ret = alarm_line_cur(line);
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
  * 功　能：更新表
  * 入口参数：
  * 返回值：无
  */
void TOTAL_ThresholdDlg::updateTable(void)
{
    if(m_totalData.TcurMax.size() > 0)
    {
        int offset = 0;
        QString str;

        for(int i=0; i<NPMV_LINE_COUNT; ++i)
        {
            offset++;

            float temp = m_totalData.Tcur.at(i)/10.0;
            str = QString::number(temp,'f', 1);
            m_itemVec.at(offset++)->setText(str);

            checkAlarm(i,m_itemVec.at(offset-1)); // 报警颜色

            if(m_initFlag == false)
            {
                temp = m_totalData.TcurMin.at(i)/10.0;
                str = QString::number(temp,'f', 1);
                m_itemVec.at(offset++)->setText(str);

                temp = m_totalData.TcurMax.at(i)/10.0;
                str = QString::number(temp,'f', 1);
                m_itemVec.at(offset++)->setText(str);
            }
            else
                offset += 2;

            break; // 只显示第一项
        }

        m_initFlag = true;
    }
}


/**
  * 功　能：
  * 入口参数：
  * 返回值：无
  */
void TOTAL_ThresholdDlg::checkUpdateTable(void)
{
    updateTable();
}


/**
  * 功　能：初始化入口函数
  * 入口参数：
  * 返回值：无
  */
void TOTAL_ThresholdDlg::InitFunction(void)
{
    timeoutDone();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}


/**
  * 功　能：定时器响应函数
  * 入口参数：
  * 返回值：无
  */
void TOTAL_ThresholdDlg::timeoutDone(void)
{
    m_totalData = npm_get_pduData()->totalData; // 获取数据
    if(m_totalData.TcurMax.size() > 0) // 已经收取到数据
    {
        checkUpdateTable();
    }
}


/**
  * 功　能：日志保存
  * 入口参数：
  * 返回值：无
  */
 void TOTAL_ThresholdDlg::slaveRecord(void)
 {
     QString str = tr("总电流阀值修改");
     QString msg = tr("修改范围:%1A~%2A").arg(m_tcurMin.at(0)/10.0).arg(m_tcurMax.at(0)/10.0);
     db_record_save(str,msg);
 }

void TOTAL_ThresholdDlg::setCurValue(void)
{
    NPM_SetUnit data;
    data.unit = NPM_TCUR_MIN;
    data.data = m_tcurMin;
    npm_set_threshold(data);

    data.data.clear();
    data.unit = NPM_TCUR_MAX;
    data.data = m_tcurMax;
    npm_set_threshold(data);

    slaveRecord();
}


void TOTAL_ThresholdDlg::on_setThresholdBtn_clicked()
{
    if(m_totalData.TcurMax.size() > 0)  // 已经收取到数据
    {
        if(m_tcurMax.size() == 0)
        {
            m_tcurMin = m_totalData.TcurMin;
            m_tcurMax = m_totalData.TcurMax;
        }

        TOTAL_ThresholdSet *dlg = new TOTAL_ThresholdSet(this);
        dlg->setData(m_tcurMin, m_tcurMax);

        int ret = dlg->exec();
        if( ret == QDialog::Accepted)
        {
            dlg->getData(m_tcurMin, m_tcurMax);
            m_totalData.TcurMin = m_tcurMin;
            m_totalData.TcurMax = m_tcurMax;

            m_initFlag = false;
            updateTable();

            ui->saveBtn->setEnabled(true);
        }
        delete dlg;
    }
    else
        QMessageBox::information(this,tr("提示"),tr("\n数据正在初始化，请等待几秒！\n"));
}


void TOTAL_ThresholdDlg::on_saveBtn_clicked()
{
    if(ui->setThresholdBtn->isEnabled())
    {
        setCurValue();

        TOTAL_WaitDlg *dlg = new  TOTAL_WaitDlg(this);
        dlg->exec();
        delete dlg;

        ui->saveBtn->setEnabled(false);
    }
}

void TOTAL_ThresholdDlg::on_refreshBtn_clicked()
{
    m_tcurMax.clear();
    m_initFlag = false;

    updateTable();
    ui->saveBtn->setEnabled(false);
}

void TOTAL_ThresholdDlg::on_quitBtn_clicked()
{
    bool ret = ui->saveBtn->isEnabled();
    if(ret)
    {
        QMessageBox msgBox(this);
         com_setBackColour(tr("信息提示"),&msgBox);
        msgBox.setText(tr("\n您已修改阀值，是否要放弃修改？\n"));
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
