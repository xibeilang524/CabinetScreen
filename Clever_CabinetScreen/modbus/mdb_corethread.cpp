/**
  * modbus 核心处理类
  *         Lzy       2016-6-2
  */
#include "mdb_corethread.h"
#include "db_sqlquery.h"

static QMutex mutex; // 互拆锁
static int s_connect_status=0; // 连接状态标志   -1 设备文件打开出错，-2 握手错误  >4 数据线错误
static NPM_sPDUData g_pduData; /*全局变量*/
static QQueue<NPM_SetUnit> g_queData;

int get_connect_status(void)
{
    return s_connect_status;
}

/* 调用这个函数进行设置*/
void npm_set_threshold(NPM_SetUnit &data)
{
    QMutexLocker locker(&mutex);
    g_queData.enqueue(data);
}

/*调用这个函数获取设备数据*/
void npm_get_pduData(NPM_sPDUData &data)
{
    QMutexLocker locker(&mutex);
    data = g_pduData;
}

NPM_sPDUData *npm_get_pduData(void)
{
    return &g_pduData;
}


MDB_CoreThread::MDB_CoreThread(QObject *parent) :
    QThread(parent)
{
    m_addr = 1;
    runFlag = false;
    m_trans = new Serial_Trans(this);

    m_readThread = new MDB_ReadThread(this);
    m_writeThread = new MDB_writeThread(this);

    //QTimer::singleShot(5*1000,this,SLOT(InitFunction()));
}

MDB_CoreThread::~MDB_CoreThread()
{
    runFlag = false;
    wait();

    delete m_writeThread;
    delete m_readThread;
    delete m_trans;
}

void MDB_CoreThread::timeoutDone(void)
{

}

void MDB_CoreThread::connectDev(void)
{
    bool ret = true;
    do
    {
        ret = m_readThread->startThread(m_trans, m_addr);
        if(runFlag == false)
            break;

        if(ret==false)
        {
            s_connect_status = -2; // 握手错误
            sleep(1);
        }
        else
            s_connect_status = 0;
    }while(ret == false);

    if(ret)
        m_writeThread->startThread(m_trans, m_addr);
}

/**
  * 功　能：读线程入口
  * 返回值：
  */
void MDB_CoreThread::run()
{    
    runFlag = true;

    connectDev();
     msleep(200);
    while(runFlag)
    {
        if(g_queData.size() > 0)
        {
            QMutexLocker locker(&mutex);
            m_que = g_queData.dequeue();
            m_writeThread->setThreshold(m_que); // 设置PDU
        }
        m_writeThread->writeThreshold();

        bool ret = m_readThread->readData(); // 读取PDU设备数据
        if(ret)
        {
            getData(g_pduData);
            s_connect_status = 0; //连接正常
        }
        else
        {
            if(s_connect_status<5)
            {
                s_connect_status++; // 错误次数增加
                if(s_connect_status == 5) // 产生日志
                {
                    sql_sRecordLog log;
                    log.type = tr("连接状态");
                    log.msg = tr("异常（请检查PDU设备、数据连接线） ");
                    sql_insert_record(log);
                }
            }
            msleep(200);
        }
    }
}

/**
  * 功　能：开启线程
  * 返回值：
  */
bool MDB_CoreThread::startThread(QString com, uchar addr)
{
    bool ret = m_trans->openSerial(com);
    if(ret)
    {
        m_addr = addr;
        start();
    }
    else
    {
        s_connect_status = -1;  // 标志设备文件打开错误
        m_trans->closeSerial();
    }

    return ret;
}

void MDB_CoreThread::getData(NPM_sPDUData &pduData)
{
    QMutexLocker locker(&mutex);
    m_readThread->getData(pduData);
}
