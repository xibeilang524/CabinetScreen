/**
  *  npmv modbus rtu协议   读取命令处理
  *         Lzy       2016-6-1
  */
#include "mdb_recvcmd.h"

static QMutex mutex; // 互拆锁
static NPM_RTUR_sData rtuData;    /*全局变量防止反复开辟空间*/

/**
  * 功　能： 接收到的数据进行检验
  * 入口参数：array-> 接收缓冲区
  * 出口参数：
  * 返回值： true
  */
bool npmv_recv_check(uchar *array)
{
    uchar buf[] = {0x01, 0x83, 0x03, 0x01, 0x31};

    int ret = memcmp(array,buf,sizeof(array));
    if(ret == 0) {
        qDebug() <<" npmv  recv check error";
        return false;
    }
    return true;
}


/**
  * 功　能： 接收到的数据进行解析
  * 入口参数：array-> 接收缓冲区
  * 出口参数：data -> 解析完成的数据
  * 返回值：数据长度
  */
int npmv_recv_data(uchar *array, NPM_RTUR_sData &data)
{
    uint offsets = 0;

    if(npmv_recv_check(array) == false)
        return 0;

    data.addr = array[offsets++]; // 从机地址（1）
    data.fn = array[offsets++]; // 功能码（1）

    //qDebug() << array[0] << array[1]<<array[2] << array[3]<<array[4] << array[5]<<array[2];
    // data.len = (array[offsets++] << 8); // 数据字节数（2） 没有高位
    data.len = array[offsets++];

    memset(data.data, 0, sizeof(data.data));
    for(int i=0; i<data.len; ++i)       //数据（N）
        data.data[i] = array[offsets++];

    ushort CRC = crc_16(array, offsets);   // 获得校验码
    data.crc = array[offsets++] ; // 数据字节数（2）
    data.crc += (array[offsets++] << 8);

    if(CRC != data.crc) {
        offsets = 0;
        qDebug() << "MDB recv data CRC error!";
    }

    return offsets;
}


/**
  * 功　能： 接收到的数据放入数组
  * 入口参数： buf -> 数据包地址
  * 出口参数：recvData
  * 返回值：数据长度
  */
int mdb_recv_data(uchar *buf,  QVector <uchar> &recvData)
{
    QMutexLocker locker(&mutex);
    int ret = npmv_recv_data(buf,rtuData);  // 接收到的数据进行解析
    if(ret > 0)
    {
            if(recvData.size() < rtuData.len) // 数组容量检查
                recvData.resize(rtuData.len);
            for(int i=0; i<rtuData.len; ++i)
                recvData[i] = rtuData.data[i];
    }    else  {
//        recvData.clear();
        return 0;
    }

    return recvData.size();
}



/**
  * 功　能： 接收到的数据放入数组
  * 入口参数： buf -> 数据包地址
  * 出口参数：recvData
  * 返回值：数据长度
  */
int mdb_recv_data(uchar *buf,  QVector <short> &recvData)
{
    QMutexLocker locker(&mutex);
    int ret = npmv_recv_data(buf,rtuData);  // 接收到的数据进行解析
    if(ret > 0){
        if(recvData.size() < rtuData.len/2) // 数组容量检查
            recvData.resize(rtuData.len/2);

        short temp, j=0, k=0;
        for(int i=0; i<rtuData.len; i+=2){
            temp = rtuData.data[k++] <<8;   // 高位电流
            temp += rtuData.data[k++];  // 低位电流
            recvData[j++] = temp;
            //  qDebug() <<  temp << recvData[j-1] ;
        }
    }    else  {
//        recvData.clear();
        return 0;
    }

    return recvData.size();
}


/**
  * 功　能：  接收到的数据放入数组
  * 入口参数： buf -> 数据包地址
  * 出口参数：cur
  * 返回值：数据长度
  */
int mdb_recv_data(uchar *buf,  QVector <int> &recvData)
{
    QMutexLocker locker(&mutex);
    int ret = npmv_recv_data(buf,rtuData);  // 接收到的数据进行解析
    if(ret > 0)
    {
        if(recvData.size() < rtuData.len/4) // 数组容量检查
            recvData.resize(rtuData.len/4);

        int temp, j=0, k=0;
        for(int i=0; i<rtuData.len; i+=4)
        {
            temp = rtuData.data[k++] <<24;   // 高位电流
            temp += rtuData.data[k++] << 16;  // 低位电流
            temp += rtuData.data[k++] <<8;   // 高位电流
            temp += rtuData.data[k++];  // 低位电流
            recvData[j++] = temp;
        }
    }    else  {
//        recvData.clear();
        return 0;
    }

    return recvData.size();
}




