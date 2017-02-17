#include "agvrx.h"

agvRx::agvRx(QObject *parent) : QThread(parent)
{
    M_timer1 = new QTimer(this);
    M_timer2 = new QTimer(this);
    m_ReciveCnt = 0;
    me = new MODBUSActive();
    memset(m_tagBackAgvState.tagPara,0,sizeof(m_tagBackAgvState.tagPara));
    memset(m_tagBackPosition.tagPara,0,sizeof(m_tagBackPosition.tagPara));
    memset(m_tagBackSpdAndSensor.tagPara,0,sizeof(m_tagBackSpdAndSensor.tagPara));
    bagvcomState =false;
}

void agvRx::receiveDataSlot(QByteArray buffer,int cnt)
{
//    QByteArray qsendPara;
    if(buffer.count() != 0)
    {
        if(((m_ReciveCnt == 0) && (buffer.at(0) == 0x55)) || (m_ReciveCnt > 0))
        {
            for(int i = 0; i < cnt; i++)
            {
                m_qReceiveBuffer.append(buffer.at(i));
                m_ReciveCnt++;
                if(m_ReciveCnt >= REC_BACKLENGTH)
                {
                    m_ReciveCnt = 0;
                    SciUnpack(m_qReceiveBuffer);
                    m_qReceiveBuffer.clear();
                    break;
                }
            }
        }
    }
}

void agvRx::AgvRx_ReceiveAnswerSlot(QByteArray QReceivePara, ushort uSendCnt)
{
    if(qNeedAnswerFrame.isEmpty())
    {
       qNeedAnswerFrame = QReceivePara;
       M_timer2->singleShot(200,this,SLOT(AgvRx_ReceiveAnswer_Time2OutSlot()));
    }
}

void agvRx::AgvRx_ReceiveAnswer_Time2OutSlot()
{
    M_timer2->stop();
    //100ms timeout
    if(!qNeedAnswerFrame.isEmpty())
    {
        me->ucModbusErroCn++;
        if(me->ucModbusErroCn > 3)// com failed
        {
            //signal modbustx start send
            bagvcomState = false;
            emit EnableTxSend_Sig(true);
            me->ucModbusErroCn = 0;
            qNeedAnswerFrame.clear();
        }
        else //Need resend
        {
            emit SendData_Sig(qNeedAnswerFrame,qNeedAnswerFrame.count());
            M_timer2->singleShot(200,this,SLOT(AgvRx_ReceiveAnswer_Time2OutSlot()));
            //signal modbustx stop send
            emit EnableTxSend_Sig(false);
        }
    }
}

//########################################################################################
// Function Name:	SciUnpack
// Version:			V1.0
// Input:			SciSendFrame, RecvBuff
// Output:			none
// Description:		接收完成后,将接受缓存中的数据解析到对应的结构中
//########################################################################################
void agvRx::SciUnpack(QByteArray RecvBuff)
{
    TagheadCmd tagbackheadCmd;
    int iheadLength;
    if(RecvBuff.count() == REC_BACKLENGTH) //接收帧长度
    {
        iheadLength = sizeof(tagbackheadCmd.tagheadCmd);
        for(int i = 0;i < iheadLength ; i++)
        {
            tagbackheadCmd.tagheadCmd[i] = RecvBuff.at(i);
        }
        if((tagbackheadCmd.field.head.filed.first == 0x55)
                && (tagbackheadCmd.field.head.filed.secnod == 0x77)
                && (tagbackheadCmd.field.cmd == 0x03))
        {
            switch (tagbackheadCmd.field.index) {
            case 1:
                memcpy(m_tagBackPosition.tagPara,RecvBuff.data(),REC_BACKLENGTH);
                //参数相等不做处理，参数不等进行处理
                if(memcmp(m_tagBackPosition.tagPara,tagAgvBackPosition.tagPara,REC_BACKLENGTH) != 0)
                {
                    memcpy(tagAgvBackPosition.tagPara,m_tagBackPosition.tagPara,REC_BACKLENGTH);
                }
                bagvcomState = true;
                qNeedAnswerFrame.clear();
                emit EnableTxSend_Sig(true);
                M_timer2->stop();
                break;
            case 2:
                memcpy(m_tagBackSpdAndSensor.tagPara,RecvBuff.data(),REC_BACKLENGTH);
                if(memcmp(m_tagBackSpdAndSensor.tagPara,tagAgvBackSpdAndSensor.tagPara,REC_BACKLENGTH) != 0)
                {
                    memcpy(tagAgvBackSpdAndSensor.tagPara,m_tagBackSpdAndSensor.tagPara,REC_BACKLENGTH);
                }
                emit EnableTxSend_Sig(true);
                qNeedAnswerFrame.clear();
                bagvcomState = true;
                M_timer2->stop();
                break;
            case 3:
                memcpy(m_tagBackAgvState.tagPara,RecvBuff.data(),REC_BACKLENGTH);
                if(memcmp(m_tagBackAgvState.tagPara,tagAgvBackAgvState.tagPara,REC_BACKLENGTH) != 0)
                {
                    memcpy(tagAgvBackAgvState.tagPara,m_tagBackAgvState.tagPara,REC_BACKLENGTH);
                    agvMapPara_Statehandle();
                    emit agv_stateChanged_Sig();
                }
                break;
            default:
                break;
            }
        }
    }
}
/*******当前机器人位置*************/
float agvRx::agvRx_CurX() const
{
    return m_tagBackPosition.field.x.d;
}
float agvRx::agvRx_CurY() const
{
    return m_tagBackPosition.field.y.d;
}
float agvRx::agvRx_CurA() const
{
    return m_tagBackPosition.field.A.d;
}
float agvRx::agvRx_CurW() const
{
    return m_tagBackPosition.field.w.d;
}
//当前前进速度
float agvRx::agvRx_CurSpd() const
{
    return m_tagBackSpdAndSensor.field.forwardSpd.d;
}
//当前旋转速度
float agvRx::agvRx_RotationSpd() const
{
    return m_tagBackSpdAndSensor.field.rotationSpd.d;
}
//左前超生
ushort agvRx::agvRx_leftFwdUltrasonic() const
{
    return m_tagBackSpdAndSensor.field.leftFwdUltrasonic.d;
}
//右前超生
ushort agvRx::agvRx_rightFwdUltrasonic() const
{
    return m_tagBackSpdAndSensor.field.rightFwdUltrasonic.d;
}
//左后超生
ushort agvRx::agvRx_leftBwdUltrasonic() const
{
    return m_tagBackSpdAndSensor.field.leftBckUltrasonic.d;
}
//右后超生
ushort agvRx::agvRx_rightBwdUltrasonic() const
{
    return m_tagBackSpdAndSensor.field.rightBckUltrasonic.d;
}
//左碰撞
ushort agvRx::agvRx_leftCollision() const
{
    return m_tagBackSpdAndSensor.field.leftCollision;
}
//右碰撞
ushort agvRx::agvRx_rightCollision() const
{
    return m_tagBackSpdAndSensor.field.rightCollision;
}
//左轮抬起
ushort agvRx::agvRx_leftWheelNoContact() const
{
    return m_tagBackSpdAndSensor.field.leftWheelNoContact;
}
//右轮抬起
ushort agvRx::agvRx_rightWheelNoContact() const
{
    return m_tagBackSpdAndSensor.field.rightWheelNoContact;
}
//机器人状态
ushort agvRx::agvRx_agvState() const
{
    return m_tagBackAgvState.field.state;

}

