#include "log_showdlg.h"
#include "ui_log_showdlg.h"

LOG_ShowDlg::LOG_ShowDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LOG_ShowDlg)
{
    ui->setupUi(this);
     com_setBackColour(tr("故障分析"),this);

#if defined(ARM_LINUX)
    setWindowState(Qt::WindowMaximized);
#endif

    initRecordTable();
}

LOG_ShowDlg::~LOG_ShowDlg()
{
    delete ui;
}


/**
 * @brief 初始化纪录日志表
 */
void LOG_ShowDlg::initRecordTable(void)
{
    recordTable = new LOG_TableView(ui->groupBox);
    recordTable->initRecordTable();

    connect(ui->refreshBtn,SIGNAL(clicked()),recordTable,SLOT(refreshSlot()));
    connect(ui->delBtn,SIGNAL(clicked()),recordTable,SLOT(delBtnSlot()));
    connect(ui->clearBtn,SIGNAL(clicked()),recordTable,SLOT(clearBtnSlot()));
}
