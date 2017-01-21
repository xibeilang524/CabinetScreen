/**
  * 串口测试程序（仅开发时测试用）
  *         Lzy       2016-6-1
  */
#include "serial_test.h"

Serial_Test::Serial_Test(QObject *parent) :
    QThread(parent)
{
    runFlag = false;

    m_trans.openSerial(SERIAL_COM1);
    // connect(&m_trans,SIGNAL(serialReadSig), this,SLOT(readDataSlot()));

    start();
}


Serial_Test::~Serial_Test()
{
    runFlag = false;
    wait();
}

void Serial_Test::readDataSlot()
{
    int ret = m_trans.recvData(readBuf, sizeof(readBuf));
    if(ret == 256)
        qDebug() << "serial OK!";
    else
        qDebug() << "serial err!";
}

/**
  * 功　能：测试线程入口
  * 返回值：
  */
void Serial_Test::run()
{
    runFlag = true;
    while(runFlag)
    {
        for(int i=0; i<256; ++i)
            readBuf[i] = i;
        m_trans.sendData(readBuf, sizeof(readBuf));
        msleep(500);

        readDataSlot();
        msleep(500);
    }
}
