#include "agvcom.h"
AGVCom::AGVCom(QObject *parent) : QObject(parent)
{
    m_Agvspcom = new SPcom();
//    m_spcom->moveToThread(m_spcom);
    m_Agvspcom->start();

    m_AgvRx = new agvRx();
    m_AgvRx->moveToThread(m_AgvRx);
    m_AgvRx->start();

    m_AgvTx = new AGVTx();
    m_AgvRx->moveToThread(m_AgvRx);
    m_AgvRx->start();

    m_curagvfwdspd = 10.24;
    connect(m_AgvTx,SIGNAL(SendData_Sig(QByteArray,ushort)),m_Agvspcom,SLOT(writeDataSlot(QByteArray,ushort)));
    connect(m_Agvspcom,SIGNAL(DataReciveOverSig(QByteArray,int)),m_AgvRx,SLOT(receiveDataSlot(QByteArray,int)));
    connect(this,SIGNAL(writeDataSig(QByteArray,ushort)),m_AgvTx,SLOT(recSendBufferSlot(QByteArray,ushort)));

    connect(m_AgvTx,SIGNAL(WaitRecieve_Sig(QByteArray,ushort)),m_AgvRx,SLOT(AgvRx_ReceiveAnswerSlot(QByteArray,ushort)));
    connect(m_AgvRx,SIGNAL(EnableTxSend_Sig(bool)),m_AgvTx,SLOT(AGV_EnableTxSend_Slot(bool)));
}

void AGVCom::setportName(const QString &name)
{
    m_Agvspcom->setPortName(name);
}

QString AGVCom::portName()const
{
    return m_Agvspcom->PortName();
}

int AGVCom::baudRate() const
{
    return m_Agvspcom->BaudRate();
}

void AGVCom::setbaudRate(int baudRate)
{
    m_Agvspcom->setBaudRate(baudRate);
}

bool AGVCom::isOpen()const
{
    return m_Agvspcom->isOpen();
}

bool AGVCom::open()
{
    return m_Agvspcom->open();
}

void AGVCom::close()
{
    m_Agvspcom->close();
}

bool AGVCom::clear()
{

    return m_Agvspcom->clear();
}

int AGVCom::writeData()
{
    char cBuffer[10]={0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A};
    return m_Agvspcom->writeData(cBuffer,10);
}

int AGVCom::readData(char *buffer, int count, int timeout)
{
    return m_Agvspcom->readData(buffer,count,timeout);
}

int AGVCom::write(char ch)
{
//    return m_spcom->write(ch);
    return 0;
}
float AGVCom::agvfwdspd()const
{
    return m_AgvRx->agvRx_CurSpd();
}
void AGVCom::setAgvfwdspd(float agvfwdspd)
{
    Tag_setMotion tagSetMotion;
    QByteArray qSendByte;
    memset(tagSetMotion.tagPara,0,sizeof(tagSetMotion.tagPara));
    tagSetMotion.field.tag_head.field.head.filed.first = 0x55;
    tagSetMotion.field.tag_head.field.head.filed.secnod = 0x77;
    tagSetMotion.field.tag_head.field.cmd = 0x01;
    tagSetMotion.field.tag_head.field.index = 0x01;
//    m_curagvfwdspd = agvfwdspd;
    tagSetMotion.field.forword.d = agvfwdspd;
    for(int i = 0; i < 28; i++)
    {
        qSendByte.append(tagSetMotion.tagPara[i]);
    }
    emit writeDataSig((QByteArray)qSendByte,28);
}
/***************机器人当前位置******************/
float AGVCom::CurX() const
{
    return m_AgvRx->agvRx_CurX();
}
float AGVCom::CurY() const
{
    return m_AgvRx->agvRx_CurY();
}
float AGVCom::CurA() const
{
    return m_AgvRx->agvRx_CurA();
}
float AGVCom::CurW() const
{
    return m_AgvRx->agvRx_CurW();
}
//左前超生
ushort AGVCom::leftFwdUltrasonic() const
{
    return m_AgvRx->agvRx_leftFwdUltrasonic();
}
//右前超生
ushort AGVCom::rightFwdUltrasonic() const
{
    return m_AgvRx->agvRx_rightFwdUltrasonic();
}
//左后超生
ushort AGVCom::leftBwdUltrasonic() const
{
    return m_AgvRx->agvRx_leftBwdUltrasonic();
}
//右后超生
ushort AGVCom::rightBwdUltrasonic() const
{
    return m_AgvRx->agvRx_rightBwdUltrasonic();
}
//左碰撞
ushort AGVCom::leftCollision() const
{
    return m_AgvRx->agvRx_leftCollision();
}
//右碰撞
ushort AGVCom::rightCollision() const
{
    return m_AgvRx->agvRx_rightCollision();
}
//左轮抬起
ushort AGVCom::leftWheelNoContact() const
{
    return m_AgvRx->agvRx_leftWheelNoContact();
}
//右轮抬起
ushort AGVCom::rightWheelNoContact() const
{
    return m_AgvRx->agvRx_rightWheelNoContact();
}
//机器人状态
ushort AGVCom::agvState() const
{
    return m_AgvRx->agvRx_agvState();
}


void AGVCom::recSendDataSlot(QByteArray buffer, int cnt)
{
    emit recOverSig(buffer,cnt);
}



void AGVCom::setSpdRotation(float spd, float rotation)
{
    Tag_setMotion tagSetMotion;
    QByteArray qSendByte;
    tagSetMotion.field.forword.d = spd;
    tagSetMotion.field.rotation.d = rotation;
    for(int i = 0; i < 28; i++)
    {
        qSendByte.append(tagSetMotion.tagPara[i]);
    }
    emit writeDataSig((QByteArray)qSendByte,28);
}

//*******************************************************************************
//* State Name     :  MODBUS_Distill
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
void AGVCom::agvRec_CtrData_Slot(QByteArray qSendData)
{
  emit writeDataSig(qSendData,AGVSET_LENGTH);
}

bool AGVCom::agvComState() const
{
  return m_AgvRx->bagvcomState;
}
