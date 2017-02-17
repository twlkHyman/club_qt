#ifndef SPCOM_H
#define SPCOM_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QStateMachine>
#include <QState>
#include <fcntl.h>
#include <stdio.h>
#include <QtSerialPort/QtSerialPort>
#include "modbus.h"
class SPcom : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString PortName READ PortName WRITE setPortName NOTIFY PortNameChanged)
    Q_PROPERTY(int BaudRate READ BaudRate WRITE setBaudRate NOTIFY BaudRateChanged)
    Q_PROPERTY(bool isOpen READ isOpen)
public:
    explicit SPcom(QObject *parent = 0);
    ~SPcom();

    bool isOpen() const;

    void setPortName(const QString &name);
    QString PortName()const;

    void setBaudRate(int baudRate);
    int BaudRate() const;

    Q_INVOKABLE virtual bool open();
    Q_INVOKABLE virtual bool openRead();
    Q_INVOKABLE virtual bool openWrite();
    Q_INVOKABLE virtual void close();
    Q_INVOKABLE virtual bool clear();

    int readData(char *buffer,int count,int timeout = 1000);
    int writeData(char *data,int size);
    int write(char ch);
    Uint16 SciFrameCheck(QByteArray Recvbuff);
//    QString arr;
signals:
    void PortNameChanged();
    void BaudRateChanged();

    void DataReciveOverSig(QByteArray buffer,int cnt);
protected:
    void run();
    QString m_portName;
    int m_baudRate;
    QSerialPort *m_serialPort;
    QTimer  *m_timer;
    char m_readBufer[50];
    int m_readLen;
    int m_success;
    int m_failed;
    Tag_SciRecvFrame  m_gSciRecvFrame;                                              // 接收帧结构信息
public slots:
    void writeDataSlot(QByteArray buffer,ushort cnt);
    void readDataProtocal();
    void readyReadSlot();
    void spcomErr(QSerialPort::SerialPortError error);
    void timeoutSlot();
};

//Uint16 Sub_CalCRC(Uint16* CrcPointer, Uint16 Length);
#endif // SPCOM_H

//========================================================================================
// 变量定义
//========================================================================================
//Tag_SciInformation    gSciInformation;
//Tag_SciSendFrame  gSciSendFrame;                                              // 发送帧结构信息

//Tag_SciTimer      gSciTimer;

//Uint16 gRecvBuff[MB_ADU_MAX_SIZE];                                                // 接收数据缓存
//Uint16 gSendBuff[MB_ADU_MAX_SIZE];                                                // 发送数据缓存

//Uint16 SciTxCnt;
//StructTypeSciProtocTag stSciProtoc;

////========================================================================================
//// 函数声明
////========================================================================================
//void  SCI_init(void);
//void  SciTimerInit(void);
//void  SciTransmitDriver(Uint16 sciIndex, Uint16 sciTxData);
//Uint16    SciReceiveDriver(Uint16 sciIndex, Uint16* sciRxData);
//void  SciDriver(void);
//void  SciTimer15(void);
//void  SciTimer(void);

//Uint16 ReadHoldingRegisters(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff);
//Uint16 ReadInputRegisters(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff);
//Uint16 ReadWriteMultipleRegisters(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff);

//Uint16 WriteSingleRegister(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff);
//Uint16 WriteMultipleRegisters(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff);

//void  SciSlaveState(void);
//Uint16 SciFrameCheck(Uint16 *Recvbuff);
//void  SciUnpack(Tag_SciSendFrame *SciSendFrame, Uint16 *Recvbuff);
//Uint16 SciProcessRequest(Tag_SciSendFrame *SciSendFrame, Tag_SciRecvFrame RecvFrame);
//void SciPack(Tag_SciSendFrame *SendFrame, Uint16* SendBuff);

