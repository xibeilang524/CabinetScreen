#ifndef MDB_READTHREAD_H
#define MDB_READTHREAD_H

#include <QThread>
#include "mdb_recvcmd.h"

class MDB_ReadThread : public QThread
{
    Q_OBJECT
public:
    explicit MDB_ReadThread(QObject *parent = 0);
    ~MDB_ReadThread();

    bool startThread(Serial_Trans *trans,uchar addr=1);
    bool readData(void);
    void getData(NPM_sPDUData &pduData);

protected:
    void run();

    bool loopbackTest(uchar addr);
    bool autoUpdate(NPM_sPDUData &pduData,uchar addr);
    void autoUpdateThread(NPM_sPDUData &pduData,uchar addr);

private:
    int transData(int len, int mdelay=300);
    int updateData(NPM_CMD_sData &data,QVector <uchar> &recvData);
    int updateData(NPM_CMD_sData &data,QVector<short> &recvData);
    int updateData(NPM_CMD_sData &data,QVector<int> &recvData);

    int updateCKSData(NPM_sCurKwh  &, uchar addr);
    int updateCurThreshold(NPM_sCurKwh  &, uchar addr);

    int updateTotalCur(NPM_sTotalData  &data, uchar addr);
    int updateTotalCurThreshold(NPM_sTotalData  &data, uchar addr);

    int updateEnv(NPM_sAlarm  &data, uchar addr);
    int updateEnvThreshold(NPM_sAlarm  &data, uchar addr);

    int updateAlarm(NPM_sAlarm  &data, uchar addr);
    int updateDev(NPM_sDevInfo  &dev, uchar addr);
    int updateIP(NPM_sDevInfo  &dev, uchar addr);

signals:
    
public slots:
    void readDataSlot();
    
private:
    bool runFlag; /*线程标志*/
    int m_step, m_stepThreshold;

    uchar m_readBuf[312];
    NPM_CMD_sData m_sentData;
    NPM_sPDUData m_PDUData;

    Serial_Trans *m_trans;
};

#endif // MDB_READTHREAD_H
