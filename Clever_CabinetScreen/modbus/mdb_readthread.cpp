/**
  * 读线程
  *         Lzy       2016-4-28
  */
#include "mdb_readthread.h"
static int toalElec =0;


MDB_ReadThread::MDB_ReadThread(QObject *parent) :
    QThread(parent)
{
    runFlag = false;
    m_step = m_stepThreshold = 0;

    //m_trans = new Serial_Trans(this);
    // connect(&trans,SIGNAL(serialReadSig), this,SLOT(readDataSlot()));
}

MDB_ReadThread::~MDB_ReadThread()
{
    runFlag = false;
    wait();
   // delete m_trans;
}

/**
  * 功　能：数据传送出去
  * 入口参数：len -> 长度
  * 返回值：接收数据长度
  */
int MDB_ReadThread::transData(int len, int mdelay)
{
    int ret = m_trans->sendData(m_readBuf, len);   // 发送数据
    if(ret > 0)
    {
        msleep(mdelay);
        memset(m_readBuf, 0, sizeof(m_readBuf));
        ret = m_trans->recvData(m_readBuf, sizeof(m_readBuf));  // 接收数据
        if(ret <= 0)
            qDebug() << "trans DatapduData err" << ret;
    }

    return ret;
}

/**
  * 功　能：modbus回环测试
  * 入口参数：addr -> 从机地址
  * 返回值：true
  */
bool MDB_ReadThread::loopbackTest(uchar addr)
{
    int len =  npmv_sent_lookback(addr, m_readBuf);   // 获取回环命令
    int ret = transData(len);
    if(ret > 0)
    {
        ret = npmv_recv_lookback(addr,m_readBuf); // 验证回环数据
        if(ret == 0) {
            qDebug() << "loop back test OK";
            return true;
        }
        else
            qDebug() << "loop back test err";
    }

    return false;
}

/**pduData
  * 功　能：数据更新
  * 入口参数：
  * 返回值： > 0
  */
int MDB_ReadThread::updateData(NPM_CMD_sData &data,QVector <uchar> &recvData)
{
    int  ret = mdb_sent_cmd(data);
    ret = transData(ret,800);  // 获取IP 时间要久一些
    if(ret <= 0) {
        qDebug() << "update  uchar Data err" << ret;
        return -1;
    }
    else
        ret = mdb_recv_data(m_readBuf,recvData);

    return  ret;
}

/**
  * 功　能：数据更新
  * 入口参数：
  * 返回值： > 0
  */
int MDB_ReadThread::updateData(NPM_CMD_sData &data,QVector <short> &recvData)
{
    int  ret = mdb_sent_cmd(data);
    ret = transData(ret);
    if(ret <= 0) {
        qDebug() << "update short Data err" << ret;
        return -1;
    }
    else
        ret = mdb_recv_data(m_readBuf,recvData);

    return  ret;
}

/**
  * 功　能：数据更新
  * 入口参数：
  * 返回值： > 0
  */
int MDB_ReadThread::updateData(NPM_CMD_sData &data,QVector <int> &recvData)
{
    int  ret = mdb_sent_cmd(data);
    ret = transData(ret);
    if(ret <= 0) {
        qDebug() << "update int  Data err" << ret;
        return -1;
    }
    else
        ret = mdb_recv_data(m_readBuf,recvData);

    return  ret;
}

/**
  * 功　能：数据更新电流、电能、输出开关状态
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateCKSData(NPM_sCurKwh  &data ,uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = NPMV_OUT_COUNT;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_CUR;
    int ret = updateData(m_sentData,data.cur); // 更新电流
    if(ret <= 0){
        qDebug() << "update cur err";
        return ret;
    }

        m_sentData.reg = NPMV_SWICTH;
        ret = updateData(m_sentData, data.swicth); // 输出单元的开关状态
        if(ret <= 0){
            qDebug() << "update switch err";
            return ret;
        }

    m_sentData.reg = NPMV_KWH;
    m_sentData.num = NPMV_OUT_COUNT*2;
    ret = updateData(m_sentData, data.kwh); // 更新电能
    if(ret <= 0){
        qDebug() << "update kwh err";
        return ret;
    }
    else
    {
        toalElec = 0;
        for(int i=0; i<data.kwh.size(); ++i)
            toalElec += data.kwh.at(i);
    }

    return ret;
}

/**
  * 功　能：更新电流阀值
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateCurThreshold(NPM_sCurKwh  &data, uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = NPMV_OUT_COUNT;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_CUR_MIN;
    int ret = updateData(m_sentData,data.curMin); // 获取电流下限
    if(ret <= 0){
        qDebug() << "update curMin err";
        return ret;
    }

    m_sentData.reg = NPMV_CUR_MAX;
    ret = updateData(m_sentData,data.curMax); // 获取电流上限
    if(ret <= 0){
        qDebug() << "update cur Max err";
        return ret;
    }

    return ret;
}

/**
  * 功　能：更新总电流
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateTotalCur(NPM_sTotalData  &data, uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = NPMV_LINE_COUNT;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_TCUR;
    int ret = updateData(m_sentData,data.Tcur); // 获取总电流
    if(ret <= 0){
        qDebug() << "update total cur err";
        return ret;
    }
    //qDebug() << data.Tcur[0]<< data.Tcur[1]<< data.Tcur[2] ;

    m_sentData.reg = NPMV_TVOL;
    ret = updateData(m_sentData,data.Tvol); // 获取电压
    if(ret <= 0){
        qDebug() << "update total vol  err";
        return ret;
    }
    //qDebug() << data.Tvol[0]<< data.Tvol[1]<< data.Tvol[2] ;

    m_sentData.reg = NPMV_TPF;
    ret = updateData(m_sentData,data.Tpf); // 获取总功率因素
    if(ret <= 0){
        qDebug() << "update total PF  err";
        return ret;
    }

    m_sentData.reg = NPMV_TKWH;
    m_sentData.num = NPMV_LINE_COUNT*2;
    ret = updateData(m_sentData,data.TKwh); // 获取总电能
    if(ret <= 0){
        qDebug() << "update total kwh err";
        return ret;
    }
    else
        data.TKwh[0] =  toalElec; // 总电能
//     qDebug() << data.Tvol[0]<< data.Tvol[1]<< data.Tvol[2] ;

    return ret;
}

/**
  * 功　能：更新总电流阀值
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateTotalCurThreshold(NPM_sTotalData  &data, uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = NPMV_LINE_COUNT;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_TCUR_MIN;
    int ret = updateData(m_sentData,data.TcurMin); // 获取总电流下限
    if(ret <= 0){
        qDebug() << "update total cur min err";
        return ret;
    }

    m_sentData.reg = NPMV_TCUR_MAX;
    ret = updateData(m_sentData,data.TcurMax); // 获取总电流上限
    if(ret <= 0){
        qDebug() << "update total cur  max err";
        return ret;
    }

    m_sentData.reg = NPMV_TVOL_MIN;
    ret = updateData(m_sentData,data.TvolMin); // 获取总电压下限
    if(ret <= 0){
        qDebug() << "update total vol min  err";
        return ret;
    }

    m_sentData.reg = NPMV_TVOL_MAX;
    ret = updateData(m_sentData,data.TvolMax); // 获取总压上限
    if(ret <= 0){
        qDebug() << "update total vol Max err";
        return ret;
    }

    return ret;
}

/**
  * 功　能：更新环境数据
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateEnv(NPM_sAlarm  &data, uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = NPMV_TH_COUNT;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_TEMP;
    int ret = updateData(m_sentData,data.temp); // 获取温度
    if(ret <= 0){
        qDebug() << "update temp err";
        return ret;
    }
    //qDebug() << data.temp[0]<< data.temp[1]<< data.temp[2] ;

    m_sentData.reg = NPMV_HUM;
    ret = updateData(m_sentData,data.hum); // 获取湿度
    if(ret <= 0){
        qDebug() << "update hum  err";
        return ret;
    }
    //qDebug() << data.hum[0]<< data.hum[1]<< data.hum[2] ;

    m_sentData.num = 1;
    m_sentData.reg = NPMV_WATER;
    ret = updateData(m_sentData,data.water); // 获取水浸
    if(ret <= 0){
        qDebug() << "update water  err";
        return ret;
    }

    m_sentData.reg = NPMV_SMOKE;
    ret = updateData(m_sentData,data.smoke); // 获取烟雾
    if(ret <= 0){
        qDebug() << "update smoke err";
        return ret;
    }

    m_sentData.num = NPMV_DOOR_COUNT;
    m_sentData.reg = NPMV_DOOR;
    ret = updateData(m_sentData,data.door); // 获取水浸
    if(ret <= 0){
        qDebug() << "update door  err";
        return ret;
    }

    return ret;
}

/**
  * 功　能：更新环境阀值
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateEnvThreshold(NPM_sAlarm  &data, uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = NPMV_TH_COUNT;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_TH_TMIN;
    int ret = updateData(m_sentData,data.tempMin); // 获取温度下限
    if(ret <= 0){
        qDebug() << "update temp min err";
        return ret;
    }
    // qDebug() << data.tempMin[0]<< data.tempMin[1]<< data.tempMin[2] ;

    m_sentData.reg = NPMV_TH_HMIN;
    ret = updateData(m_sentData,data.humMin); // 获取湿度下限
    if(ret <= 0){
        qDebug() << "update hum min err";
        return ret;
    }
    //qDebug() << data.humMin[0]<< data.humMin[1]<< data.humMin[2] ;

    m_sentData.reg = NPMV_TH_TMAX;
    ret = updateData(m_sentData,data.tempMax); // 获取温度上限
    if(ret <= 0){
        qDebug() << "update temp max err";
        return ret;
    }
    // qDebug() << data.tempMax[0]<< data.tempMax[1]<< data.tempMax[2] ;

    m_sentData.reg = NPMV_TH_HMAX;
    ret = updateData(m_sentData,data.humMax); // 获取湿度上限
    if(ret <= 0){
        qDebug() << "update hum max err";
        return ret;
    }
    //qDebug() << data.humMax[0]<< data.humMax[1]<< data.humMax[2] ;

    return ret;
}

/**
  * 功　能：更新报警标志
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateAlarm(NPM_sAlarm  &data, uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = NPMV_ALARM_COUNT;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_ALARM_FLAG;
    int ret = updateData(m_sentData,data.alarmflag); // 获取报警标识
    if(ret <= 0){
        qDebug() << "update alarm err";
        return ret;
    }

    // 总电流进行判断  ///========   下位机没有实现，只能自己补充
    extern NPM_sPDUData *npm_get_pduData(void);
    NPM_sPDUData *pduData = npm_get_pduData();
    if(pduData->totalData.TcurMin.size() > 0)
    {
        int cur = pduData->totalData.Tcur.at(0);
        int min = pduData->totalData.TcurMin.at(0);
        int max = pduData->totalData.TcurMax.at(0);
        if((cur < min) ||( cur> max))
            data.alarmflag[24] = 1;
    }

//     qDebug() << data.alarmflag.size();
//    qDebug() << data.alarmflag[24]<< data.alarmflag[25]<< data.alarmflag[26] ;

    return ret;
}

/**
  * 功　能：获取设备信息
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateDev(NPM_sDevInfo  &dev, uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = 1;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_ONLINE;
    int ret = updateData(m_sentData,dev.online); // 获取级联标志  为1表示在级连的队列里
    if(ret <= 0){
        qDebug() << "update online err";
        return ret;
    }

    m_sentData.reg = NPMV_TYPE;
    ret = updateData(m_sentData,dev.type); // PDU类型 0-3对应单相A-D系列  5-8对应3相A-D系列
    if(ret <= 0){
        qDebug() << "update dev type err";
        return ret;
    }

    m_sentData.reg = NPMV_M_S;
    ret = updateData(m_sentData,dev.m_s); //级连时的主从机号  范围为0-4
    if(ret <= 0){
        qDebug() << "update dev m_s err";
        return ret;
    }

    /*
    m_sentData.num = NPMV_NUM_PER_SWICTH_COUNT;
    m_sentData.reg = NPMV_NUM_PER_SWICTH;
    ret = updateData(m_sentData,dev.numPerSwicth); //每个执行板单元数量 0870H-0875H
    if(ret <= 0){
        qDebug() << "update dev num Per Swicth err";
        return ret;
    }
    qDebug() << dev.numPerSwicth.size() ;
*/

    return ret;
}

/**
  * 功　能：获取设备信息
  * 入口参数：data -> 结构体
  * 返回值： > 0
  */
int MDB_ReadThread::updateIP(NPM_sDevInfo  &dev, uchar addr)
{
    m_sentData.addr = addr;
    m_sentData.num = 2;
    m_sentData.buf = m_readBuf;

    m_sentData.reg = NPMV_IP;
    int ret = updateData(m_sentData,dev.IP); //IP   倒过来的
    if(ret <= 0){
        qDebug() << "update dev IP err";
        return ret;
    }
    //qDebug() << dev.IP[3] << dev.IP[2] << dev.IP[1] << dev.IP[0];


    return ret;
}

/**
  * 功　能：数据自动更新
  * 入口参数：
  * 返回值：pduData
  */
bool MDB_ReadThread::autoUpdate(NPM_sPDUData &pduData, uchar addr)
{
    int ret = updateCKSData(pduData.curKwh, addr);  // 更新电流、电能、开关状态

    switch(++m_step)
    {
    case 1:
       ret += updateTotalCur(pduData.totalData, addr);// 更新总电流、电压、电能、功率因素、
        break;

    case 2:
        ret += updateAlarm(pduData.alarm, addr);// 标志
        break;

    default:
        m_step = 0;
        break;
    }

    if(ret > 0)
        autoUpdateThread(pduData,addr);
    else
        return false;

    return true;
}

void MDB_ReadThread::autoUpdateThread(NPM_sPDUData &pduData, uchar addr)
{
    switch( ++m_stepThreshold)
    {
    case  1: // 更新电流阀值
          updateCurThreshold(pduData.curKwh, addr);
        break;

    case 2: // 更新总阀值
          updateTotalCurThreshold(pduData.totalData, addr);
        break;

    case 3: // 更新环境数据
        updateEnv(pduData.alarm, addr);
        break;

    case 4: // 更新阀值
          updateEnvThreshold(pduData.alarm, addr);
        break;

    case 5:  // 更新设备信息
        updateDev(pduData.dev, addr);
        break;

    case 6: // 更新IP
        //updateIP(pduData.dev, addr);
        break;

    default:
        m_stepThreshold = 0;
        break;
    }
}


void MDB_ReadThread::readDataSlot()
{

}

bool MDB_ReadThread::readData(void)
{
     return autoUpdate(m_PDUData,m_sentData.addr);
}


/**
  * 功　能：读线程入口
  * 返回值：
  */
void MDB_ReadThread::run()
{
    runFlag = true;
    while(runFlag)
        autoUpdate(m_PDUData,m_sentData.addr);
}

/**
  * 功　能：开始握手
  * 返回值：
  */
bool MDB_ReadThread::startThread(Serial_Trans *trans, uchar addr)
{
    m_trans = trans;
    m_sentData.addr = addr;

    bool ret =  loopbackTest(addr);
    if(ret) {        
        //start();  这里不单独开启线程
    }
   // else
   //     m_trans->closeSerial();

    return ret;
}

/**
  * 功　能：读取数据
  * 出口参数： pduData -> PDU数据结构体
  * 返回值：
  */
void MDB_ReadThread::getData(NPM_sPDUData &pduData)
{
    pduData = m_PDUData;
}

