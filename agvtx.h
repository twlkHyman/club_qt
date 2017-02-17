#ifndef AGVTX_H
#define AGVTX_H

#include <QObject>
#include "modbus.h"
#include <QThread>
#include "agvparastruct.h"
class AGVTx : public QThread
{
    Q_OBJECT
public:
    explicit AGVTx(QObject *parent = 0);

    Tag_setMotion tagSetMotion;
    Tag_setPosition tagSetAimPosition;
    Tag_setPosition tagSetStartPosition;
    Tag_getInfo tagGetPosition;
    Tag_getInfo tagGetCurSpd;

    uchar SendBitBufferDistill(uchar *ucBitBufferPara);
    ModBusSPara  MODBUSPara_Get( MODBUSActive *me);
    void  MODBUSPara_LIFO( MODBUSActive *me,  ModBusSPara const e);
    void  MODBUSPara_FIFO( MODBUSActive *me, ModBusSPara e);
    bool  MODBUS_GetFIFOFree(void);
    bool  isAgvCheckCmd(QByteArray QReceivePara);
private:
    MODBUSActive *me;
     bool bEnableSend;
protected:
    void run();
signals:
    void SendData_Sig(QByteArray QReceivePara,ushort uSendCnt);
    void WaitRecieve_Sig(QByteArray QReceivePara,ushort uSendCnt);//发送等待接收
private slots:
    void recSendBufferSlot(QByteArray QReceivePara,ushort uSendCnt);
    void AGV_EnableTxSend_Slot(bool bEnable);// 响应rx发送改变发送使能slot
};

#endif // AGVTX_H
