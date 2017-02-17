#include "spcom.h"
//#include "calculateqthread.h"
#include "modbusrx.h"
SPcom::SPcom(QObject *parent) : QThread(parent)
{
    m_serialPort = new QSerialPort();
    m_baudRate = 9600;
    m_portName = "";
    m_timer = new QTimer(this);
    m_failed = 0;
    m_success = 0;
//    connect(m_timer,SIGNAL(timeout()),this,SLOT(readDataProtocal()));
}

SPcom::~SPcom()
{
    delete m_serialPort;
}
void SPcom::run()
{
//    while(1)
//    {
//        readDataProtocal();
//        msleep(10);
//    }
    exec();
}
void SPcom::setPortName(const QString &name)
{
    m_portName = name;
}

QString SPcom::PortName()const
{
    return m_portName;
}

int SPcom::BaudRate() const
{
    return m_baudRate;
}
static QSerialPort::BaudRate getBaudRate(int baudRate)
{
    switch(baudRate)
    {
    case 1200:
        return QSerialPort::Baud1200;
    case 2400:
        return QSerialPort::Baud2400;
    case 4800:
        return QSerialPort::Baud4800;
    case 9600:
        return QSerialPort::Baud9600;
    case 19200:
        return QSerialPort::Baud19200;
    case 38400:
        return QSerialPort::Baud38400;
    case 57600:
        return QSerialPort::Baud57600;
    case 115200:
        return QSerialPort::Baud115200;
    default:
        return QSerialPort::UnknownBaud;
    }
}

void SPcom::setBaudRate(int baudRate)
{
    m_baudRate = baudRate;
}
bool SPcom::isOpen()const
{
    return m_serialPort->isOpen();
}

bool SPcom::open()
{
    if(m_serialPort->isOpen())
    {
        return true;
    }
//    m_timer->start(10);
//    m_reader.clearError();
//           m_reader.clear();
    m_timer->start(100);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
    connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    connect(m_serialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(spcomErr(QSerialPort::SerialPortError)));
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(getBaudRate(m_baudRate));
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setReadBufferSize(1024);
    return m_serialPort->open(QSerialPort::ReadWrite);
}
bool SPcom::openRead()
{
    if(m_serialPort->isOpen())
    {
        return true;
    }
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(getBaudRate(m_baudRate));
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setReadBufferSize(1024);
    return m_serialPort->open(QSerialPort::ReadOnly);
}
bool SPcom::openWrite()
{
    if(m_serialPort->isOpen())
    {
        return true;
    }

    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(getBaudRate(m_baudRate));
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setReadBufferSize(1024);
    return m_serialPort->open(QSerialPort::WriteOnly);
}

void SPcom::close()
{
    if(m_serialPort->isOpen())
    {
        m_serialPort->close();
    }
//    m_timer->stop();
}

bool SPcom::clear()
{
    if(m_serialPort->isOpen())
    {
        m_serialPort->clear();
        this->close();
        return this->open();
    }
    return false;
}

int SPcom::readData(char *buffer,int count,int timeout)
{
    int len = 0;
    forever
    {
        int n = m_serialPort->read(&buffer[len],count - len);
        if(n == -1)
        {
            return -1;
        }
        else if((n == 0) && !m_serialPort->waitForReadyRead(timeout))
        {
            return -2;
        }
        else
        {
            len += n;
            if(count == len)
                break;
        }
    }
    return len;
}

int SPcom::writeData(char *data,int size)
{
    int len = 0;
    forever
    {
        int n = m_serialPort->write(&data[len],size - len);
        if(n == -1)
        {
            return -1;
        }
        else
        {
            len += n;
            if(size == len)
            {
                break;
            }
        }
    }
    return len;
}

void SPcom::readDataProtocal()
{
//    if(m_gSciRecvFrame.TotalRecvByteQty == 0)
//    {
//        m_readLen = readData(m_readBufer,2,10);
//        if(m_readBufer[0] == 0x01)//addr
//        {
//            if(isValidCmd(m_readBufer[1]))
//            {
//                m_gSciRecvFrame.ReadAddress = m_readBufer[0];
//                m_gSciRecvFrame.Function = m_readBufer[1];
//                m_gSciRecvFrame.TotalRecvByteQty += 2;


//            }
//        }
//        else if(m_readBufer[1] == 0x01)
//        {

//        }
//    }
   m_readLen = readData(m_readBufer,10,200);
   if(m_readLen > 0)
   {
        m_success++;
        qDebug("readData %d len is %d/n",m_success,m_readLen);
//        Sub_CalCRC((Uint16 *)m_readBufer,10);
        //check modbus protocal
        emit DataReciveOverSig(m_readBufer,m_readLen);
   }
   else
   {
       if(m_readLen == -1)
       {
           qDebug("readData len is -1",m_failed);
       }
       else if(m_readLen == -2)
       {
           qDebug("readData len is -2",m_failed);
       }
       else
       {
           qDebug("readData len is 0",m_failed);
       }
        m_failed++;
   }
}

//########################################################################################
// Function Name:	SciFrameCheck
// Version:			V1.0
// Input:			Recvbuff
// Output:			TempFrameState
// Description:		帧检测; CRC和帧长度
//########################################################################################
Uint16 SPcom::SciFrameCheck(QByteArray Recvbuff)
{
    Uint16 TempCrcData		= 0;
    Uint16 TempFrameState	= 0;
    Uint16 TempFrameLength	= 0;

    switch (Recvbuff[1])
    {
    case MB_FUNCTION_REAND_COILS:
    case MB_FUNCTION_REAND_DISCRETE_INPUTS:
    case MB_FUNCTION_REAND_HOLDING_REGS:
    case MB_FUNCTION_WRITE_SINGLE_COILS:
    case MB_FUNCTION_WRITE_SINGLE_REGS:
        TempFrameLength = 8;
        break;
    case MB_FUNCTION_REAND_INPUT_REGS:				// 为了避免03号节点误接收其它从站返回的读指令
        TempFrameLength = 100;
        break;
    case MB_FUNCTION_WRITE_MULTIPLE_COILS:
    case MB_FUNCTION_WRITE_MULTIPLE_REGS:
        TempFrameLength = Recvbuff[6] + 9;
        break;
    case MB_FUNCTION_REANDWRITE_MULTIPLE_REGS:
        TempFrameLength = Recvbuff[10] + 13;
        break;
    default:
        break;
    }

    if ((m_gSciRecvFrame.TotalRecvByteQty == TempFrameLength) && (TempFrameLength > 2))
    {
        //CRC计算
        TempCrcData = Sub_CalCRC(Recvbuff, (TempFrameLength - 2));

        //移位操作有问题，乘以256
        if(TempCrcData == (Recvbuff[TempFrameLength - 1] * 256 + Recvbuff[TempFrameLength - 2]))
        {
            TempFrameState = MB_FRAME_OK;
        }
        else
        {
            TempFrameState = MB_FRAME_CRC_ERR;
        }
    }
    else
    {
        TempFrameState = MB_FRAME_LENGTH_ERR;
    }

    return TempFrameState;
}

void SPcom::readyReadSlot()
{
    m_success++;
    qDebug() << m_success;
    QByteArray arr = m_serialPort->readAll();
    emit DataReciveOverSig(arr,arr.count());
    qDebug() << arr ;
//    qDebug() << m_success;
//    do_something(arr);
}

void SPcom::spcomErr(QSerialPort::SerialPortError error)
{
    if(error)
    {
        ;
    }
    else
    {
        ;
    }
}

void SPcom::timeoutSlot()
{
    ;
}

void SPcom::writeDataSlot(QByteArray buffer,ushort cnt)
{
    writeData((char *)buffer.data(),cnt);
}
