/**
  *  npmv modbus rtu协议   读取命令处理
  *         Lzy       2016-6-1
  */
#include "mdb_sentcmd.h"

/**
  * 功　能：CRC检验程序  校验码会自动填充至缓冲区
  * 入口参数：arrar -> 校验数据  len 检验数据的长度
  * 出口参数：
  * 返回值：CRC值
  */
ushort crc_16(uchar *arrar,uint len)
{
    /**
    CRC-16(循环冗余错误校验)
      冗余循环码(CRC)包含2个字节，即16位二进制。CRC码由发送设备
    计算，放置于发送信息的尾部。接收信息的设备再重新计算接收到信
    息的CRC码，比较计算得到的CRC码是否与接收到的相符，如果两者不
    相符，则表明出错。
      在进行CRC码计算时只用8位数据位。起始位、停止位、奇偶校验
    位都不参与CRC码计算。
      计算CRC码的步骤为：
      ① 预置16位寄存器为十六进制FFFF(即全为1)。称此寄存器为CRC寄
    存器。
      ② 把第一个8位数据与16位CRC寄存器的低位相异或运算，把结果
    放于CRC寄存器。
      ③ 把寄存器的内容右移一位，用0填补最高位，检查最低位。
      ④ 如果最低位为0：重复第3步，再次右移一位；如果最低位为1
    ：CRC寄存器与多项式A001(1010 0000 0000 0001)进行异或运算。
      ⑤ 重复步骤③和④，直到右移8次，这样整个8位数据全部进行了
    处理。
      ⑥ 重复步骤②到⑤，进行下个8位数据的处理。
      ⑦ 最后得到的CRC寄存器的值即为CRC码。
      ⑧ 将CRC码分成高8位和低8位，按低位在先，高位在后，将它们
    加到传送数据之后。
      */
    uint  IX,IY,CRC=0xFFFF;

    if (len>0)
    {
        len--;
        for (IX=0;IX<=len;IX++)
        {
            CRC=CRC^(uint)(arrar[IX]);
            for(IY=0;IY<=7;IY++)
            {
                if ((CRC&1)!=0 )
                    CRC=(CRC>>1)^0xA001;
                else
                    CRC=CRC>>1;    //
            }
        }
    }
    else
        CRC = 0;

    return CRC;
}


/**
  * 功　能：CRC测试程序程序
  * 返回值：
  */
void crc_test(void)
{
    uchar arrar[8] = {0x01, 0x08, 0x00, 0x00, 0x12, 0x34};
    ushort crc = crc_16(arrar, sizeof(arrar)-2);
    qDebug("0x%x ", crc);
}



/**
  * 功　能： 结构体数据填充至缓冲区中
  * 入口参数：data -> 发送的结构体
  * 出口参数：buf -> 填充好的数据缓冲区
  * 返回值：数据长度
  */
int npmv_sent_data(NPM_RTUS_sData &data, uchar *buf)
{
    uint offsets = 0;

    buf[offsets++] = data.addr; // 填写地址
    buf[offsets++] = data.fn; // 填写功能码

    // 填写寄存器
    buf[offsets++] = (data.reg>>8); // 高8位
    buf[offsets++] = (data.reg&0xff); // 低8位

    //填写寄存器数量
    buf[offsets++] = (data.num>>8); // 高8位
    buf[offsets++] = (data.num&0xff); // 低8位

    data.crc = crc_16(buf, offsets);
    buf[offsets++] = (data.crc & 0x00ff);  //低位置
    buf[offsets++]= (data.crc & 0xff00)>>8;//高位置

    return offsets;
}


/**
  * 功　能：发送回路命令包
  * 入口参数：addr -> 从机地址
  * 返回值：true
  */
int npmv_sent_lookback(uchar addr, uchar *array)
{
    NPM_RTUS_sData data;

    data.addr = addr;
    data.fn = 0x08;
    data.reg = 0;
    data.num = 0x1234;

    return npmv_sent_data(data, array);
}


/**
  * 功　能：接收回路命令包
  * 入口参数：addr -> 从机地址
  * 返回值：0 正常   -1 异常
  */
int npmv_recv_lookback(uchar addr, uchar *buf)
{
    uchar arrayErr[] ={0x01, 0x89, 0x01, 0x86, 0x50}; /*应答信息(异常时)*/
    uchar array[] = {0x00, 0x08, 0x00, 0x00, 0x12, 0x34, 0xED, 0x7C};
    array[0] = addr;

    int ret = memcmp(array,buf,sizeof(array));
    if(ret)  /* 异常时*/
    {
        ret = memcmp(arrayErr,buf,sizeof(arrayErr));
        if(ret){
            ret = -1;
            qDebug() << "MDB recv looback CRC Error!";
        }
        else
            qDebug() << "MDB recv looback CRC abnormal!";
    }

    return ret;
}

/**
  * 功　能： 发送命令函数
  * 入口参数：addr-> 地址  reg-> 寄存器   num-> 位数
  * 出口参数：buf -> 数据包地址
  * 返回值：数据长度
  */
int mdb_sent_cmd(NPM_CMD_sData &sentData)
{
    NPM_RTUS_sData data;

    data.addr = sentData.addr; // 地址
    data.fn =  NPMV_FN_READ; // 功能码读  sentData.fn
    data.reg =sentData. reg; // 输出单元电流
    data.num = sentData.num; // 输出位数

    return npmv_sent_data(data,sentData.buf);
}





