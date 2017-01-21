#ifndef SERIAL_TRANS_H
#define SERIAL_TRANS_H

/**
  * 串口基本操作 打开、关闭、读写
  */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <QObject>
#include <qsocketnotifier.h>
#include <sys/termios.h>
#include <QDebug>
#include <QMutexLocker>
#include "serial_portset.h"


class Serial_Trans : public QObject
{
    Q_OBJECT
public:
    explicit Serial_Trans(QObject *parent = 0);
    ~Serial_Trans();
    
    bool openSerial(QString);
    int getSerialFd(void);
    void closeSerial();

    int sendData(unsigned char *pBuff, int nCount);
    int recvData(unsigned char *pBuf, int nCount);

signals:
    void serialReadSig();
    void serialClosed();

protected  slots:
    void readDataSlot();

private:
    int fd; //串口句柄
    QMutex mutex; // 互拆锁
    QSocketNotifier * monitor;
};

#endif // SERIAL_TRANS_H
