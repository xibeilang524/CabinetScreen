#ifndef SERIAL_TEST_H
#define SERIAL_TEST_H

#include <QThread>
#include "serial_trans.h"

class Serial_Test : public QThread
{
    Q_OBJECT
public:
    explicit Serial_Test(QObject *parent = 0);
    ~Serial_Test();

protected:
    void run();

signals:

public slots:
    void readDataSlot();

private:
    bool runFlag; /*线程标志*/

    uchar readBuf[256];
    Serial_Trans m_trans;
};

#endif // SERIAL_TEST_H
