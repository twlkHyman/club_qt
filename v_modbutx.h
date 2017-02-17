#ifndef V_MODBUTX_H
#define V_MODBUTX_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <fcntl.h>
#include <stdio.h>
#include <QtSerialPort/QtSerialPort>
#include "modbus.h"




class v_modbutx : public QThread
{
    Q_OBJECT
public:
    explicit v_modbutx(QObject *parent = 0);

    uchar SendBitBufferDistill(uchar *ucBitBufferPara);
    ModBusSPara  MODBUSPara_Get( MODBUSActive *me);
    void  MODBUSPara_LIFO(  MODBUSActive *me,  ModBusSPara const e);
    void  MODBUSPara_FIFO( MODBUSActive *me, ModBusSPara e);
    bool  MODBUS_GetFIFOFree(void);
private:
    MODBUSActive *me;
protected:
    void run();
signals:
    void SendData_Sig(QByteArray QReceivePara,ushort uSendCnt);
private slots:
    void recSendBufferSlot(QByteArray QReceivePara,ushort uSendCnt);
};

#endif // V_MODBUTX_H
