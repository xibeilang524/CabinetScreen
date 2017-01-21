#ifndef MDB_WRITETHREAD_H
#define MDB_WRITETHREAD_H

#include <QQueue>
#include "mdb_readthread.h"

enum _NPM_WRITE_CMD
{
    NPM_CUR_MIN, //输出单元电流最小值
     NPM_CUR_MAX, //输出单元电流最大值

    NPM_TCUR_MIN, // 总电流最小值
    NPM_TCUR_MAX, // 总电流最大值

    NPM_TVOL_MIN, // 总电压最小值
    NPM_TVOL_MAX, // 总电压最大值

    NPM_TH_TMIN, // 温度最小值
    NPM_TH_TMAX, //温度最大值

    NPM_SWICTH, //输出单元的开关状态
};


/*设置单元结构体*/
struct _NPM_SetUnit{
    uchar addr; // 从机地址
    short    unit; // 功能标识符
    QVector <short> data;
};
typedef struct _NPM_SetUnit  NPM_SetUnit;


class MDB_writeThread : public QThread
{
    Q_OBJECT
public:
    explicit MDB_writeThread(QObject *parent = 0);

    bool startThread(Serial_Trans *trans, uchar addr);
    void setThreshold(NPM_SetUnit &unit);
    bool writeThreshold(void);

protected:
    bool transData(QVector <short> &data);

signals:
    void setErrSig();

public slots:


private:
    Serial_Trans *m_trans;
    QQueue<NPM_SetUnit> m_que;

    int m_delay;
    NPM_Write_sData m_writeData;
    uchar m_buf[NPM_BUF_SIZE];
};

#endif // MDB_WRITETHREAD_H
