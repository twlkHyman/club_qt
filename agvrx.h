#ifndef AGVRX_H
#define AGVRX_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "agvparastruct.h"
#include "modbus.h"
#include <QByteArray>
#define REC_BACKLENGTH 30
class agvRx : public QThread
{
    Q_OBJECT
public:
    explicit agvRx(QObject *parent = 0);

    uint8 m_receivedBuffer[MB_ADU_MAX_SIZE];
    void SciUnpack(QByteArray RecvBuff);


    //当前前进速度
    float agvRx_CurSpd() const;
    //当前旋转速度
    float agvRx_RotationSpd() const;
    /*******当前机器人位置*************/
    float agvRx_CurX() const;
    float agvRx_CurY() const;
    float agvRx_CurA() const;
    float agvRx_CurW() const;
    //左前超生
    ushort agvRx_leftFwdUltrasonic() const;
    //右前超生
    ushort agvRx_rightFwdUltrasonic() const;
    //左后超生
    ushort agvRx_leftBwdUltrasonic() const;
    //右后超生
    ushort agvRx_rightBwdUltrasonic() const;
    //左碰撞
    ushort agvRx_leftCollision() const;
    //右碰撞
    ushort agvRx_rightCollision() const;
    //左轮抬起
    ushort agvRx_leftWheelNoContact() const;
    //右轮抬起
    ushort agvRx_rightWheelNoContact() const;
    //机器人状态
    ushort agvRx_agvState() const;
    bool bagvcomState;
private:
    MODBUSActive *me;
    QTimer *M_timer1;
    QTimer *M_timer2;
    QByteArray qNeedAnswerFrame;            //需要应答
    QByteArray m_qReceiveBuffer;              //接收缓冲
    Tag_backAgvState m_tagBackAgvState;
    Tag_backPosition m_tagBackPosition;
    Tag_backSpdAndSensor m_tagBackSpdAndSensor;
    int m_ReciveCnt;
signals:
    void ReciveData_Sig(QByteArray QReceivePara,ushort bFirstSend);
    void SendBuffer_Sig(QByteArray QReceivePara,ushort uSendCnt);
    void SendData_Sig(QByteArray QReceivePara,ushort uSendCnt); // 通知串口发送对象发送数据
    void EnableTxSend_Sig(bool bEnable);
    void agvRx_PositionChange_Sig();
    void agv_stateChanged_Sig();
//    void SendVoice_Sig(qVect16 QVoice,ushort uSendCnt);
private slots:
    void receiveDataSlot(QByteArray buffer,int cnt);
    void AgvRx_ReceiveAnswerSlot(QByteArray QReceivePara,ushort uSendCnt);
    void AgvRx_ReceiveAnswer_Time2OutSlot();

};

#endif // AGVRX_H
