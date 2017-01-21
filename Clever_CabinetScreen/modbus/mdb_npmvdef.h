/**
  * npmv 协议宏定义
  *   Lzy       2016-6-2
  */
#ifndef MDB_NPMVDEF_H
#define MDB_NPMVDEF_H

#define     NPMV_FN_READ        0x03  // 功能码读
#define     NPMV_FN_WRITE       0x10  // 功能码写

#define     NPMV_OUT_COUNT  24  //输出位

#define     NPMV_CUR                   0x000  //输出单元电流  0000H-0017H     =register/10    int cur[0][24]
#define     NPMV_CUR_MIN           0x100 //输出单元电流下限 0100H-0117H
#define     NPMV_CUR_MAX        0x130 //输出单元电流上限    0130H-0147H

#define     NPMV_LINE_COUNT    3    // 三相

#define     NPMV_TCUR_MIN       0x200   //总电流下限 0200H-0202H
#define     NPMV_TCUR_MAX      0x203    //总电流上限 0203H-0205H

#define     NPMV_TVOL_MIN       0x206 //总电压下限 0206H-0208H
#define     NPMV_TVOL_MAX       0x209 //总电压上限 0209H-020BH

#define     NPMV_TCUR               0x20C //总电流 020CH-020EH
#define     NPMV_TVOL               0x20F //总电压 020FH-0211H

#define     NPMV_TH_COUNT   4

#define     NPMV_TH_TMIN         0x300 // 温度下限
#define     NPMV_TH_HMIN        0x304 // 湿度下限
#define     NPMV_TH_TMAX      0x310 // 温/湿度上限
#define     NPMV_TH_HMAX      0x314 // 温/湿度上限

#define     NPMV_TEMP    0x320  // 温度传感器 0320H-0323H
#define     NPMV_HUM    0x328   // 0328H-032BH

#define     NPMV_WATER  0X400   //  水浸
#define     NPMV_SMOKE  0X410   // 烟雾

#define     NPMV_DOOR   0X420 // 门禁 0420H-0421H
#define     NPMV_DOOR_COUNT 2

#define     NPMV_KWH    0X500 // 输出单元电能  0500H-052EH
#define     NPMV_TPF       0X600 // 总功率因数 0600H-0602H
#define     NPMV_TKWH   0X610 // 总电能  0610H-0615H

#define     NPMV_SWICTH     0X700   // 输出单元的开关状态 0700H-0717H
#define     NPMV_OUT_NUM    0X800 // 输出单元的数量

#define    NPMV_ONLINE  0X810  // 级联标志   为1表示在级连的队列里
#define    NPMV_TYPE        0X820 // PDU类型 0-3对应单相A-D系列  5-8对应3相A-D系列

#define     NPMV_M_S        0X830  // 级连时的主从机号  范围为0-4

#define     NPMV_ALARM_COUNT    (24+3+8+4)
#define     NPMV_ALARM_FLAG     0X840 //  报警标识  0840H-0866H 24个输出位，3个相电流，8个温湿度，2个门禁，1个烟雾，1个水浸的报警
#define     NPMV_ALARM_BIT      NPMV_ALARM_FLAG     // 24个输出位
#define     NPMV_ALARM_LCUR  (NPMV_ALARM_BIT + 24) // 3个相电流
#define     NPMV_ALARM_TH       (NPMV_ALARM_LCUR + 3) // 8个温湿度
#define     NPMV_ALARM_DOOR     (NPMV_ALARM_TH + 8) // 2个门禁
#define     NPMV_ALARM_SMOKE    (NPMV_ALARM_DOOR + 2) // 1个烟雾
#define     NPMV_ALARM_WATER    (NPMV_ALARM_SMOKE + 1) // 1个水浸的报警

#define     NPMV_NUM_PER_SWICTH     0X870   // 每个执行板单元数量 0870H-0875H
#define     NPMV_NUM_PER_SWICTH_COUNT     6

#define     NPMV_IP     0X900       // IP地址


#endif // MDB_NPMVDEF_H
