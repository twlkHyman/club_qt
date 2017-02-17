#ifndef MODBUSRX_H
#define MODBUSRX_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <fcntl.h>
#include <stdio.h>
#include <QtSerialPort/QtSerialPort>
#include "modbus.h"
//===============================================================================
//Para Structure Definition
//===============================================================================
#define MODBUS_RECEIVE_LENGTH           10		//
typedef union
{
    struct
    {
        uchar 		ucNodeId;			//
        uchar  		ucFuction;			//
        tag_Data	DataAdd;			//
        tag_Data	Data;				//
        tag_Data	Crc;				//
        ushort	    Reserved;
    }field;
    uchar ucUsartTx[MODBUS_RECEIVE_LENGTH];
}tag_UsartTx;

#define MODBUS_READ_RESPONSE_LENGTH        	0x07
typedef union
{
    struct
    {
        uchar 		ucNodeId;			//
        uchar  		ucFuction;			//
        uchar		ucDataNum;			//
        tag_Data	Data;				//
        tag_Data	Crc;				//
    }field;
    uchar ucUsartRdAck[MODBUS_READ_RESPONSE_LENGTH];
}tag_UsartRdAck;


#define MODBUS_WRITE_RESPONSE_LENGTH        	0x08
//
typedef union
{
    struct
    {
        uchar 		ucNodeId;			//
        uchar  		ucFuction;			//
        tag_Data	DataAdd;			//
        tag_Data	Data;				//
        tag_Data	Crc;				//
    }field;
    uchar ucUsartWrAck[MODBUS_WRITE_RESPONSE_LENGTH];
}tag_UsartWrAck;





//===============================================================================
//Class Definition
//===============================================================================
class ModbusRx : public QThread
{
    Q_OBJECT
//    Q_PROPERTY(QString PortName READ PortName WRITE setPortName NOTIFY PortNameChanged)
//    Q_PROPERTY(int BaudRate READ BaudRate WRITE setBaudRate NOTIFY BaudRateChanged)
//    Q_PROPERTY(bool isOpen READ isOpen)
public:
    explicit ModbusRx(QObject *parent = 0);
    ~ModbusRx();
    void OpenSerialPort(void);

    Uint16 SciFrameCheck (Uint16* Recvbuff);
    Uint16 SciFrameCheck(QByteArray Recvbuff);
    Uint16 WriteSingleRegister(Tag_SciRecvFrame* RecvFrame, QByteArray Recvbuff);
    Uint16 ReadHoldingRegisters(Tag_SciRecvFrame *RecvFrame, QByteArray Recvbuff);
    void SciUnpack(Tag_SciSendFrame* SciSendFrame,QByteArray RecvBuff);
    void SciPack(Tag_SciSendFrame *SendFrame, Uint16* SendBuff);
    Uint16 SciProcessRequest(Tag_SciSendFrame* SciSendFrame,Tag_SciRecvFrame RecvFrame);

    Tag_SciInformation    gSciInformation;
    Uint16 m_receivedBuffer[MB_ADU_MAX_SIZE];
    Uint16 m_sendBuffer[MB_ADU_MAX_SIZE];
    bool    m_comState;
    QTimer *M_timer1;
    QTimer *M_timer2;
signals:
    void SendData_Sig(QByteArray QReceivePara,ushort uSendCnt);
    void ReciveData_Sig(QByteArray QReceivePara,ushort bFirstSend);
    void StateChange_Sig();
    void paraAdd_Sig();
    void step_over_sig();
    void CheckBuffer_Sig();
    void ReadPara_Sig();
    void ModifyPara_Sig();
    void ReadRespond_Sig();
    void ModifyRespond_Sig();
    void IdleState_Sig();
    void ErrState_Sig();

    void EnableTxSend_Sig(bool);

protected:
    void run();
private:
    QByteArray qNeedAnswerFrame;            //需要应答
    Tag_SciRecvFrame  gSciRecvFrame;          // 接收帧结构信息
    Tag_SciSendFrame  *gSciSendFrame;
    MODBUSActive *me;

public slots:
     void receiveDataSlot(QByteArray buffer,int cnt);
     void PostreceiveDataSlot(QByteArray QReceivePara,bool bFirstSend);
     void MODBUS_Top_Entered();
     void MODBUS_Top_ReceiveAnswer(QByteArray QSendPara,ushort usParaStatus);
     void MODBUS_Top_Time1Out();



     void MODBUS_Idle_Entered();
     void MODBUS_Idle_Timout();
     void MODBUS_Idle_exited();

//     void MODBUS_Distill_Entered();
//     void MODBUS_Distill_Timeout();

     void MODBUS_Wait_Entered();
     void MODBUS_Wait_Timeout();


     void WaitReceive_Slot(QByteArray QQReceivePara,ushort cnt);
};

#endif // MODBUSRX_H
