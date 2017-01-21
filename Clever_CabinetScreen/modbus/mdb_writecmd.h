#ifndef MDB_WRITECMD_H
#define MDB_WRITECMD_H
#include "mdb_sentcmd.h"


/*npmV  写数据结构体*/
struct _NPM_Write_sData
{
    uchar addr; // 从机地址（1）
    uchar fn; // 功能码（1）
    ushort reg; // 寄存器首地址（2）
    ushort num; // 个数（2）
    uchar len; // 数据字节数
    uchar data[312]; // 数据区域
    ushort crc; // CRC码（2）
    uchar *buf;
};
typedef struct _NPM_Write_sData  NPM_Write_sData;
int npmv_write_data(NPM_Write_sData &data);
bool npmv_write_check(uchar *buf, int len);

#endif // MDB_WRITECMD_H
