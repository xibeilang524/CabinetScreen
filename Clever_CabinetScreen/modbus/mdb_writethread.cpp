/**
  *  modbus 写寄存器类
  *         Lzy       2016-6-1
  */
#include "mdb_writethread.h"

MDB_writeThread::MDB_writeThread(QObject *parent) :
    QThread(parent)
{
    m_delay = 3;
    m_writeData.addr = 1;
    m_writeData.fn = NPMV_FN_WRITE;
    m_writeData.buf = m_buf;
}


/**
  * 功　能：数据传送出去
  * 入口参数：data -> 数据
  * 返回值：true
  */
bool MDB_writeThread::transData(QVector <short> &data)
{
     short k=0, temp;

     m_writeData.len = data.size() * 2;  // 设置发送长度
     for(int i=0; i<data.size(); ++i) // 填充数据
     {
         temp = data.at(i);

         m_writeData.data[k++] = ((temp>>8) &0x00ff); // 取高8位
         m_writeData.data[k++] = (temp &0xff);         // 取低8位
     }
     int ret = npmv_write_data(m_writeData);

    msleep(500); // 写之前等等
    ret = m_trans->sendData(m_buf, ret);   // 发送数据
    if(ret > 0)
    {
        sleep(m_delay);
        memset(m_buf, 0, sizeof(m_buf));
        ret = m_trans->recvData(m_buf, sizeof(m_buf));  // 接收数据
        if(ret != 0)
        {
            qDebug() << "trans write err" << ret;
            return npmv_write_check(m_buf, ret);
        }
    }

    return false;
}


/**
  * 功　能：写线程函数 此函数由线程调用 用于异步完成数据发送
  * 入口参数：无
  * 返回值：true  设置错误时，会发出错误信号
  */
bool MDB_writeThread::writeThreshold(void)
{
    m_delay = 3;
    NPM_SetUnit unit;

    if( m_que.size() > 0)
    {
        unit = m_que.dequeue();
        //m_writeData.addr = unit.addr;

        switch(unit.unit)
        {
        case NPM_CUR_MIN: // 设置输出单元电流最小值
            m_writeData.reg = NPMV_CUR_MIN;
            m_writeData.num = NPMV_OUT_COUNT;
            m_delay = 14;
            break;

        case NPM_CUR_MAX: // 设置输出单元电流最大值
            m_writeData.reg = NPMV_CUR_MAX;
            m_writeData.num = NPMV_OUT_COUNT;
             m_delay = 14;
            break;

        case NPM_TCUR_MIN: // 设置总电流最小值
            m_writeData.reg = NPMV_TCUR_MIN;
            m_writeData.num = NPMV_LINE_COUNT;
            break;

        case NPM_TCUR_MAX: // 设置总电流最大值
            m_writeData.reg = NPMV_TCUR_MAX;
            m_writeData.num = NPMV_LINE_COUNT;
            break;

        case NPM_TVOL_MIN: // 设置总电压最小值
            m_writeData.reg = NPMV_TVOL_MIN;
            m_writeData.num = NPMV_LINE_COUNT;
            break;

        case NPM_TVOL_MAX: // 设置总电压最大值
            m_writeData.reg = NPMV_TVOL_MAX;
            m_writeData.num = NPMV_LINE_COUNT;
            break;

        case NPM_TH_TMIN: // 设置温度最小值
            m_writeData.reg = NPMV_TH_TMIN;
            m_writeData.num = NPMV_TH_COUNT *2; // 注意 温湿度一起设置
            break;

        case NPM_TH_TMAX: // 设置温度最大值
            m_writeData.reg = NPMV_TH_TMAX;
            m_writeData.num = NPMV_TH_COUNT*2; // 注意 温湿度一起设置
            break;

        case NPM_SWICTH: // 设置输出单元的开关状态
            m_writeData.reg = NPMV_SWICTH;
            m_writeData.num = NPMV_OUT_COUNT;
            break;

        default:
            qDebug() << "write Thread unit err";
            return false;
        }

        bool ret = transData(unit.data);
        if(ret == false){
            emit setErrSig();
            qDebug() << "set err";
        }

       return  ret;
    }

   return true;
}


/**
  * 功　能：开始线程
  * 返回值：true
  */
bool MDB_writeThread::startThread(Serial_Trans *trans, uchar addr)
{
    m_trans = trans;
    m_writeData.addr = addr;

    return true;
}


/**
  * 功　能：设置函数
  * 入口参数：NPM_SetUnit -> 设置结构体
  * 返回值：无
  */
void MDB_writeThread::setThreshold(NPM_SetUnit &unit)
{
    m_que.enqueue(unit);
}
