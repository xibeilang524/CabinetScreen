#ifndef MDB_SENTCMD_H
#define MDB_SENTCMD_H

#include "serial_trans.h"
#include "mdb_npmvtype.h"


/*npmv  数据发送结构体*/
struct _NPM_RTUS_sData
{
    uchar addr; // 从机地址（1）
    uchar fn; // 功能码（1）
    ushort reg; // 寄存器首地址（2）
    ushort num; // 个数（2）
    ushort crc; // CRC码（2）
};
typedef struct _NPM_RTUS_sData  NPM_RTUS_sData;

ushort crc_16(uchar *arrar,uint len);

int npmv_sent_lookback(uchar addr, uchar *array);
int npmv_recv_lookback(uchar addr, uchar *buf);

/*npmv  数据发送结构体*/
struct _NPM_CMD_sData
{
    uchar addr; // 从机地址（1）
    uchar fn; // 功能码（1）
    ushort reg; // 寄存器首地址（2）
    ushort num; // 个数（2）
    uchar *buf; // 数据区域
};
typedef struct _NPM_CMD_sData  NPM_CMD_sData;
int mdb_sent_cmd(NPM_CMD_sData &sentData);


#endif // MDB_SENTCMD_H
