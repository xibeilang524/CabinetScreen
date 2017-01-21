/**
  * 报警线程
  *         Lzy       2016-6-2
  */
#include "alarm_thread.h"
#include "db_sqlquery.h"
static QVector <short> g_alarmflag; // 报警标识   24个输出位，3个相电流，8个温湿度，2个门禁，1个烟雾，1个水浸的报警

/**
  * 功　能：保存纪录
  * 入口参数：type
  * 返回值：无
  */
void db_record_save(QString &type, QString &msg)
{
    sql_sRecordLog log;

    log.type = type;
    log.msg = msg;
    sql_insert_record(log);
}

/**
  * 功　能：相电流报警
  * 入口参数：line -> 相数
  * 返回值：true 表示报警
  */
bool alarm_line_cur(int line)
{
    bool ret = false;
    int offset = 24+line;

    if(g_alarmflag.size() > 0)
    {
        if(g_alarmflag.at(offset))
            ret = true;
    }

    return ret;
}

/**
  * 功　能：输出电流报警
  * 入口参数：bit -> 输出位
  * 返回值：true 表示报警
  */
bool alarm_output_bit(int bit)
{
    bool ret = false;

    if(g_alarmflag.size() > 0)
    {
        if(g_alarmflag.at(bit))
            ret = true;
    }
    return ret;
}


ALARM_Thread::ALARM_Thread(QObject *parent) :
    QThread(parent)
{
    m_isRun = false;
}

ALARM_Thread::~ALARM_Thread()
{
    m_isRun = false;
    wait();
}

/**
  * 功　能：输出位报警检查
  * 入口参数：alarm -> 报警标识   24个输出位
  * 返回值：无
  */
void ALARM_Thread::outputALarm(QVector <short> &alarm)
{
    static uint alarmFlag = 0;

    sql_sRecordLog log;
    log.type = tr("输出电流报警");

    for(int i=0; i<NPMV_OUT_COUNT; ++i)
    {
        if(alarm.at(i) == 1)
        {
            if(alarmFlag & (1<<i)) // 已经被设置 已经被报警
                continue;
            else   // 需要报警
            {
                alarmFlag |= (1<<i);

                NPM_sPDUData *pduData = npm_get_pduData();
                float cur  = pduData->curKwh.cur.at(i)/10.0 ;
             //   int min = pduData->curKwh.curMin.at(i) ;
             //   int max = pduData->curKwh.curMax.at(i) ;

              //  if((cur < min) || (cur > max))
                {
                    log.msg = tr("Output%1 电流值:%2A").arg(i+1).arg(cur);
                    sql_insert_record(log);
                }
            }
        }
        else
            alarmFlag &= ~(1<<i); // 清0
    }
}

void ALARM_Thread::totalCurAlarm(QVector <short> &alarm)
{
    static uint alarmFlag = 0;
    int offset = 24;

    sql_sRecordLog log;
    log.type = tr("相电流报警");

    if(alarm.at(offset))
    {
        if(alarmFlag==0)
        {
            alarmFlag = 1;

            NPM_sPDUData *pduData = npm_get_pduData();
            float cur  = pduData->totalData.Tcur.at(0) / 10.0;
            //float min = pduData->totalData.TcurMin.at(0) / 10.0;
            // float max = pduData->totalData.TcurMax.at(0) / 10.0;
            log.msg = tr("相电流 电流值:%1A ").arg(cur);
            sql_insert_record(log);
        }
    }
    else
        alarmFlag = 0;
}


void ALARM_Thread::checkAlarm(void)
{
    NPM_sPDUData *pduData = npm_get_pduData();
    g_alarmflag = pduData->alarm.alarmflag;
    if(g_alarmflag.size()>0)
    {
        outputALarm(g_alarmflag);
        totalCurAlarm(g_alarmflag);
    }
}


void ALARM_Thread::run(void)
{
    m_isRun = true;
    while(m_isRun)
    {
        sleep(1);
        checkAlarm();
    }
}
