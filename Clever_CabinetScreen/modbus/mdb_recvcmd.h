#ifndef MDB_RECVCMD_H
#define MDB_RECVCMD_H

#include "mdb_writecmd.h"

#define NPM_BUF_SIZE   512     // 接收缓冲区

/*npmv  响应数据结构体*/
struct _NPM_RTUR_sData
{
    uchar addr; // 从机地址（1）
    uchar fn; // 功能码（1）
    ushort len; // 数据字节数（2）
    ushort data[NPM_BUF_SIZE]; // 数据（N）
    ushort crc; // CRC码（2）
};
typedef struct _NPM_RTUR_sData  NPM_RTUR_sData;

int mdb_recv_data(uchar *buf,  QVector <uchar> &recvData);
int mdb_recv_data(uchar *buf,  QVector <short> &recvData);
int mdb_recv_data(uchar *buf,  QVector <int> &recvData);

#endif // MDB_RECVCMD_H
