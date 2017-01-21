/**
  *  端口设置
  *       Lzy       2016-6-1
  */
#include "serial_portset.h"
#include <QThread>

Serial_PortSet::Serial_PortSet()
{
    /*串口信息初始化*/
    m_serialData.baudRate = SERIAL_BAUDRATE ;
    m_serialData.dataBit  = "8";
    m_serialData.parity   = "none";
    m_serialData.stopBit  ="1" ;
}

/**
  * 功　能：设置端口名
  * 入口参数：name -> 设备名
  * 出口参数：serial -> 修改后的串口信息
  * 返回值：null
  */
void Serial_PortSet::setPortName(QString &name,Serial_PortData &serial)
{
    m_serialData.devName = name;
    serial = m_serialData;
}


/**
  * 功　能：设置波特率
  * 入口参数：baudRate -> 波特率
  * 出口参数：serial -> 修改后的串口信息
  * 返回值：null
  */
 void Serial_PortSet::setBaudRate(QString baudRate,Serial_PortData &serial)
 {
    m_serialData.baudRate = baudRate;
    serial = m_serialData;
 }
