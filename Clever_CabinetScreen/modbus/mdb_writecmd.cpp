#include "mdb_writecmd.h"


/**
  * 功　能： 结构体数据填充至缓冲区中
  * 入口参数：data -> 发送的结构体
  * 出口参数：data.buf -> 填充好的数据缓冲区
  * 返回值：数据长度
  */
int npmv_write_data(NPM_Write_sData &data)
{
    uint offsets = 0;
    uchar *buf = data.buf;

    buf[offsets++] = data.addr; // 填写地址
    buf[offsets++] = data.fn; // 填写功能码

    // 填写寄存器
    buf[offsets++] = (data.reg>>8); // 高8位
    buf[offsets++] = (data.reg&0xff); // 低8位

    //填写寄存器数量
    buf[offsets++] = (data.num>>8); // 高8位
    buf[offsets++] = (data.num&0xff); // 低8位

    buf[offsets++] = data.len; // 字节数
    for(int i=0; i<data.len; ++i)
        buf[offsets++]  = data.data[i];  // 数据内容

    data.crc = crc_16(buf, offsets);
    buf[offsets++] = (data.crc & 0x00ff);  //低位置
    buf[offsets++]= (data.crc & 0xff00)>>8;//高位置

    return offsets;
}

/**
  * 功　能： 检查写数据返回码
  * 入口参数：buf -> 发送的结构体
  * 出口参数：
  * 返回值：true
  */
bool npmv_write_check(uchar *buf, int len)
{
    uchar array[] = {0x01, 0x90, 0x02, 0xCD, 0xC1};

    if(len == sizeof(array))
    {
        int ret = memcmp(buf,array,len);
        if(ret == 0)
            return false;
    }

 //   for(int i=0; i<len; ++i)
 //       qDebug("%x ",buf[i]);

    return true;
}


