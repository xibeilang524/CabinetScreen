/**
  * 串口基本操作 打开、关闭、读写
  *         Lzy       2016-6-1
  */
#include "serial_trans.h"

Serial_Trans::Serial_Trans(QObject *parent) :
    QObject(parent)
{
    fd = -1;
}

Serial_Trans:: ~Serial_Trans()
{
    closeSerial();
}

/**
  * 功　能：打开端口
  * 入口参数：serialName> 设备名
  * 出口参数：
  * 返回值：true
  */
bool Serial_Trans::openSerial(QString serialName)
{
    Serial_PortSet serialPort;
     Serial_PortData setData;
    serialPort.setPortName(serialName,setData); // 设置串口名

    struct termios setting;
    fd = open(qPrintable("/dev/" + setData.devName), O_RDWR);
    if (fd == -1) {
        //QMessageBox::warning(this, "open error", strerror(errno));
        qDebug() << "Serial Err::open "<< serialName;
        goto openerr;
    }
    tcgetattr(fd, &setting);

    //  设置波特率
    speed_t speed;
    if (setData.baudRate == "B4800") {
        speed = B4800;
    } else if (setData.baudRate == "B9600") {
        speed = B9600;
    } else if (setData.baudRate == "B19200") {
        speed = B19200;
    } else if (setData.baudRate == "B38400") {
        speed = B38400;
    } else if (setData.baudRate == "B57600") {
        speed = B57600;
    } else if (setData.baudRate == "B115200") {
        speed = B115200;
    } else if (setData.baudRate == "B230400") {
        speed = B230400;
    } else if (setData.baudRate == "B460800") {
        speed = B460800;
    } else {
        //QMessageBox::warning(this, "error", "speed error!");
        goto seterr;
    }
    cfsetispeed(&setting, speed);
    cfsetospeed(&setting, speed);
    cfmakeraw(&setting);

    //  设置数据位
    if (setData.dataBit == "8") {
        setting .c_cflag |= CS8;
    } else if (setData.dataBit == "7") {
        setting.c_cflag |= CS7;
    } else if (setData.dataBit == "6") {
        setting.c_cflag |= CS6;
    } else if (setData.dataBit == "5") {
        setting.c_cflag |= CS5;
    } else {
        //QMessageBox::warning(this, "error", "dataBit error!");
        goto seterr;
    }

    //  设置parity
    if (setData.parity == "none") {
        setting.c_cflag &= ~PARENB;
        setting.c_iflag &= ~INPCK;
    } else if (setData.parity == "odd") {
        setting.c_cflag |= (PARODD | PARENB);
        setting.c_iflag |= INPCK;
    } else if (setData.parity == "even") {
        setting.c_cflag |= PARENB;
        setting.c_cflag &= ~PARODD;
        setting.c_iflag |= INPCK;
    } else {
       // QMessageBox::warning(this, "error", "dataBit error!");
        goto seterr;
    }

    //  设置停止位
    if (setData.stopBit == "1") {
        setting.c_cflag &= ~CSTOPB;
    } else if (setData.stopBit == "2") {
        setting.c_cflag |= CSTOPB;
    } else {
        //QMessageBox::warning(this, "error", "stopBit error!");
        goto seterr;
    }

    tcflush(fd, TCIFLUSH);
    setting.c_cc[VTIME] = 0;
    setting.c_cc[VMIN] = 0;
    tcsetattr(fd, TCSANOW, &setting);

    monitor = new QSocketNotifier(fd, QSocketNotifier::Read, NULL);
    connect(monitor, SIGNAL(activated(int)), this, SLOT(readDataSlot()));
    monitor->setEnabled(true);

 //   clearData();
    return true;

seterr:
    ::close(fd);

openerr:
    return false;
}

/**
  * 功　能：关闭端口
  * 入口参数：
  * 出口参数：
  * 返回值：
  */
void Serial_Trans :: closeSerial()
{
    if(fd >= 0)
    {
        ::close(fd);
        fd = -1;

        delete monitor;
        emit serialClosed();
    }
}

/**
  * 功　能：发送数据
  * 入口参数：pBuff -> 缓冲区   nCount -> 长度
  * 返回值：发送的实际长度  <=0 出错
  */
int Serial_Trans::sendData(unsigned char *pBuff, int nCount)
{
    if(fd >= 0)
    {
        QMutexLocker locker(&mutex);

        int sendLen = write(fd, pBuff, nCount);
        fsync(fd);

        return sendLen;
    }

    return fd;
}

/**
  * 功　能：读取数据
  * 入口参数：pBuf -> 缓冲区
  * 返回值：读取的实际长度  <=0 出错
  */
int Serial_Trans::recvData(unsigned char *pBuf, int nCount)
{
    if(fd >= 0)
    {
        QMutexLocker locker(&mutex);

        int ret = read(fd, pBuf, nCount);
        return ret;
    }

    return fd;
}


 void Serial_Trans::readDataSlot()
 {
     emit serialReadSig();
 }

 /**
   * 功　能：读取设备fd
   * 返回值：
   */
  int Serial_Trans::getSerialFd(void)
  {
      return fd;
  }
