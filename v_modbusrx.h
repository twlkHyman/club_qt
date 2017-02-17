#ifndef V_MODBUSRX_H
#define V_MODBUSRX_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <fcntl.h>
#include <stdio.h>
#include <QtSerialPort/QtSerialPort>
#include "modbus.h"
class v_modbusrx : public QThread
{
    Q_OBJECT
public:
    explicit v_modbusrx(QObject *parent = 0);
    void SciUnpack(Tag_SciSendFrame* SciSendFrame,Uint16* Recvbuff);
    Uint16 SciFrameCheck(Uint16* Recvbuff);
    Uint16 WriteSingleRegister(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff);
    Uint16 WriteMultipleRegisters(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff);
    Uint16 SciProcessRequest(Tag_SciSendFrame* SciSendFrame, Tag_SciRecvFrame RecvFrame);\
    void SciPack(Tag_SciSendFrame *SendFrame, Uint16* SendBuff);
    Tag_SciInformation    gSciInformation;
    Uint16 m_receivedBuffer[MB_ADU_MAX_SIZE];
    Uint16 m_sendBuffer[MB_ADU_MAX_SIZE];
    Uint16 m_temp;
    QByteArray qSendArry;

    void emitReceiveCmdOver(Uint16 cmd,Uint16 para);
private:

    Tag_SciRecvFrame  gSciRecvFrame;          // 接收帧结构信息
    Tag_SciSendFrame  *gSciSendFrame;
//
signals:
    void ReciveData_Sig(QByteArray QReceivePara,ushort bFirstSend);
    void SendBuffer_Sig(QByteArray QReceivePara,ushort uSendCnt);
    void SendVoice_Sig(qVect16 QVoice,ushort uSendCnt);

    void vrx_ReceiveCmdOver_Sig(QByteArray cmd,QByteArray para);
private slots:
    void receiveDataSlot(QByteArray buffer,int cnt);
};
#endif // V_MODBUSRX_H
