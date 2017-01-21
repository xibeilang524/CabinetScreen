#ifndef SERIAL_PORTDATA_H
#define SERIAL_PORTDATA_H
/**
  * 串口信息定义
  */
#include "common.h"

struct _Serial_PortData
{
    QString devName;
    QString baudRate;
    QString dataBit;
    QString parity;
    QString stopBit;
};

typedef  struct _Serial_PortData  Serial_PortData ;

#endif // SERIAL_PORTDATA_H
