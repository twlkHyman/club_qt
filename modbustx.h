#ifndef MODBUSTX_H
#define MODBUSTX_H

#include <QObject>
#include <QThread>

#include "modbus.h"
#include <QStateMachine>
#include <QTImer>

class Modbustx : public QThread
{
    Q_OBJECT
public:

    explicit Modbustx(QObject *parent = 0);

    uchar SendBitBufferDistill(uchar *ucBitBufferPara);
    bool  postModbusPara(QByteArray QSendpara, OperateState opmode ,bool bFirstSend);

    QTimer *Timer1;
    QTimer *Timer2;
private:
    MODBUSActive *me;
    bool bEnableSend;
protected:
    void run();
signals:
    void StateChange_Sig();
    void CheckBuffer_Sig();
    void Validata_Modify_Sig(QByteArray QSendPara,ushort usParaStatus);
    void Validata_Read_Sig(QByteArray QSendPara,ushort usParaStatus);
    void Validata_WriteAnswer_Sig(QByteArray QSendPara,ushort usParaStatus);
    void ReadPara_Sig();
    void ModifyPara_Sig();
    void WriteAnswer_Sig();
    void IdleState_Sig();
    void chechebuffer_sig();

    void SendData_Sig(QByteArray QReceivePara,ushort uSendCnt);
    void WaitRecieve_Sig(QByteArray QReceivePara,ushort uSendCnt);
public slots:
    void MODBUS_Top_Entered();
    void MODBUS_Top_Modify(QByteArray QSendPara,ushort usParaStatus);
    void MODBUS_Top_Read(QByteArray QSendPara,ushort usParaStatus);
    void MODBUS_Top_WriteAnswer(QByteArray QSendPara,ushort usParaStatus);

    void MODBUS_Wait_Entered();
    void MODBUS_Wait_Timeout();
    void MODBUS_Idle_Entered();
    void MODBUS_Idle_Timout();
    void MODBUS_Idle_exited();
    void MODBUS_Distill_Entered();
    void MODBUS_Distill_Timeout();

    void MODBUS_ReadPara_Entered();
    void MODBUS_ModifyPara_Entered();
    void MODBUS_WriteAnswer_Entered();

    void MODBUS_Erro_Entered();

    void checkcheckSlot();

    void MODBUS_EnableTxSend_Slot(bool bEnable);
};

#endif // MODBUSTX_H
