#ifndef MDB_CORETHREAD_H
#define MDB_CORETHREAD_H

#include <QThread>
#include "mdb_writethread.h"

class MDB_CoreThread : public QThread
{
    Q_OBJECT
public:
    explicit MDB_CoreThread(QObject *parent = 0);
     ~MDB_CoreThread();

    bool startThread(QString com,uchar addr=1);
    void getData(NPM_sPDUData &pduData);

protected:
    void run();
    void connectDev(void);

signals:

public slots:
    void timeoutDone(void);

private:
     bool runFlag; /*线程标志*/

     uchar m_addr;
     NPM_SetUnit m_que;
     MDB_writeThread *m_writeThread;
     MDB_ReadThread *m_readThread;
     Serial_Trans *m_trans;
};

void npm_set_threshold(NPM_SetUnit &data);
void npm_get_pduData(NPM_sPDUData &data);
NPM_sPDUData *npm_get_pduData(void);
int get_connect_status(void);

#endif // MDB_CORETHREAD_H
