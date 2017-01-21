/**
  * 主窗口
  *         Lzy       2016-6-1
  */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "env_sensordlg.h"
#include "log_showdlg.h"
#include "timesettingdlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initWindow();

#if defined(ARM_LINUX)
    this->setWindowFlags(Qt::FramelessWindowHint);
     sleep(5);  //  延时启动
#endif

    db_connection(); // 连接数据库

    mdb = new MDB_CoreThread(this);
    mdb->startThread(SERIAL_COM1, 1);

    m_alarm = new ALARM_Thread(this);
    m_alarm->start(); // 启动报警线程

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);

    ui->envBtn->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete mdb;
    delete m_alarm;
    delete ui;
}

void MainWindow::initWindow(void)
{
    this->setAutoFillBackground(true) ;
    QPalette paletteTool = this->palette() ;
    QBrush brushToolPic(QColor(WIDGET_BACK_COLOR)) ;
    QBrush brushToolwhite(QColor(BUTTON_BACK_COLOR)) ;
    paletteTool.setBrush(QPalette::Window,brushToolPic) ;
    paletteTool.setBrush(QPalette::Base,brushToolPic) ;
    this->setPalette(paletteTool) ;

//    QPalette palette;
//    QPixmap pixmap(":/image/logo.jpg");
//    palette.setBrush(QPalette::Window, QBrush(pixmap));
//    this->setPalette(palette);
}

void MainWindow::connectStatus(void)
{
    QString str, title =  tr("连接状态：");

    int ret = get_connect_status();
    if(ret == -1) // 设备文件打开失败
        str = tr("串口打开失败!");
    else if(ret == -2)
        str = tr("与RPDU握手失败!");
    else if(ret > 4)
        str = tr("异常（请检查PDU设备、数据连接线）");
    else
        str = "";

    if(str.isEmpty())
    {
        ui->statusLab->setStyleSheet("color:black;");
        str =tr( "正常");
    }
    else
        ui->statusLab->setStyleSheet("color: rgb(255, 0, 0);");

    ui->statusLab->setText(title + str);
}


/**
  * 功　能：定时响应函数
  * 入口参数：
  * 出口参数：
  * 返回值：
  */
void MainWindow::timeoutDone(void)
{
    QString  str= QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->timeBtn->setText(str);

    connectStatus();
}

/**
  * 功　能：阀值设置
  */
void MainWindow::on_thresholdBtn_clicked()
{
    FV_MainDlg *dlg = new FV_MainDlg(this);
    dlg->exec();

    delete dlg;
}

void MainWindow::on_envBtn_clicked()
{
    ENV_SensorDlg *dlg = new ENV_SensorDlg(this);
    dlg->exec();

    delete dlg;
}

void MainWindow::on_totalBtn_clicked()
{
    TOTAL_MainDlg *dlg = new TOTAL_MainDlg(this);
    dlg->exec();

    delete dlg;
}

void MainWindow::on_outputBtn_clicked()
{
    OP_OutputDlg *dlg = new OP_OutputDlg(this);
    dlg->exec();

    delete dlg;
}

void MainWindow::on_logBtn_clicked()
{
    LOG_ShowDlg *dlg = new LOG_ShowDlg(this);
    dlg->exec();
    delete dlg;
}

void MainWindow::on_timeBtn_clicked()
{
    TimeSettingDlg *dlg = new TimeSettingDlg(this);
    dlg->exec();
    delete dlg;
}
