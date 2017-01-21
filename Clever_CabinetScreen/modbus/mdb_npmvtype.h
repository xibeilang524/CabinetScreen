/**
  * npmv 数据类型定义
  *   Lzy       2016-6-2
  */
#ifndef MDB_NPMVTYPE_H
#define MDB_NPMVTYPE_H
#include <QVector>
#include "mdb_npmvdef.h"

/*单个PDU输出单元电流、电能*/
struct _NPM_sCurKwh
{
    int outputNum; // 输出位数量

    QVector <short> cur; // 输出单元电流
    QVector <int> kwh; // 输出单元电能

    QVector <short> curMin; // 输出单元电流下限
    QVector <short> curMax; // 输出单元电流上限

    QVector <short> swicth; // 输出单元的开关状态
};
typedef struct _NPM_sCurKwh  NPM_sCurKwh;  // 电流、电能数据定义
typedef QVector <NPM_sCurKwh>  NPM_sCurKwhs;

/*总电流、电压、电能等相关信息*/
struct _NPM_sTotalData
{
    int lineNum; // 相数

    QVector <short> Tcur; //总电流
    QVector <short> Tvol; //总电压

    QVector <short> TcurMin; // 总电流下限
    QVector <short> TcurMax; // 总电流上限

    QVector <short> TvolMin; // 总电压下限
    QVector <short> TvolMax; // 总电压上限

    QVector <short> Tpf; // 总功率因数
    QVector <int> TKwh; // 总电能
};
typedef struct _NPM_sTotalData NPM_sTotalData ;
typedef QVector <NPM_sTotalData> NPM_sTotalDatas ; // 电流、电能数据定义

/* 报警数据定义 */
struct _NPM_sAlarm
{
    int num;  // 个数

    QVector <short> temp; // 温度传感器
    QVector <short> hum; // 湿度传感器

    QVector <short> tempMin; // 温度下限
    QVector <short> humMin; //  湿度下限
    QVector <short> tempMax; // 温度上限
    QVector <short> humMax; // 湿度上限

    QVector <short> water; // 水浸
    QVector <short> smoke; //烟雾

    QVector <short> door; // 门禁

    QVector <short> alarmflag; // 报警标识   24个输出位，3个相电流，8个温湿度，2个门禁，1个烟雾，1个水浸的报警
};
typedef struct _NPM_sAlarm NPM_sAlarm ;
typedef QVector <NPM_sAlarm> NPM_sAlarms ; // 电流、电能数据定义

/* 设备信息 */
struct _NPM_sDevInfo
{
    QVector <short> online; // 级联标志 为1表示在级连的队列里
    QVector <short> type;  // PDU类型 0-3对应单相A-D系列    5-8对应3相A-D系列

    QVector <short> m_s;  //级连时的主从机号 范围为0-4
    QVector <short> numPerSwicth; // 每个执行板单元数量

    QVector <uchar> IP; // IP地址 4个字节表示IP，为网络字节序
};
typedef struct _NPM_sDevInfo  NPM_sDevInfo;
typedef QVector <NPM_sDevInfo> NPM_sDevInfos ;

/*设备数据结构体*/
struct _NPM_sPDUData
{
    NPM_sDevInfo  dev;  // 设备信息

    NPM_sCurKwh curKwh;  // 输出量
    NPM_sTotalData totalData; // 总量

    NPM_sAlarm alarm; // 报警
};
typedef _NPM_sPDUData NPM_sPDUData ;
typedef QVector <NPM_sPDUData> NPM_sPDUDatas ;

#endif // MDB_NPMVTYPE_H
