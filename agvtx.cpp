#include "agvtx.h"

AGVTx::AGVTx(QObject *parent) : QThread(parent)
{
    me = new MODBUSActive();
//    me->SParaPool.ring = me->RingBuffer;
    me->SParaPool.end = MODBUSRINGBUFFERLEN;
    me->SParaPool.head = 0;
    me->SParaPool.tailSe = 0;
    me->SParaPool.nfree = MODBUSRINGBUFFERLEN;
    me->SParaPool.nMin = MODBUSRINGBUFFERLEN;

    me->ucReceiveIndex = 0;
    me->ucSendIndex = 0;
    me->ucModbusErroCn = 0;

    bEnableSend = true;
    //===========================================================
    //参数初始化
    //===========================================================
//    /******************运动控制：01****************************/
//    memset(tagSetMotion.tagPara,0,sizeof(tagSetMotion.tagPara));
//    tagSetMotion.field.tag_head.field.head = 0x5577;
//    tagSetMotion.field.tag_head.field.cmd = 0x01;
//    tagSetMotion.field.tag_head.field.index = 0x01;
//    /******************设定目标位置：02****************************/
//    memset(tagSetAimPosition.tagPara,0,sizeof(tagSetAimPosition.tagPara));
//    tagSetAimPosition.field.tag_head.field.head = 0x5577;
//    tagSetAimPosition.field.tag_head.field.cmd = 0x01;
//    tagSetAimPosition.field.tag_head.field.index = 0x02;
//    /******************设定初始位置：03****************************/
//    memset(tagSetStartPosition.tagPara,0,sizeof(tagSetStartPosition.tagPara));
//    tagSetStartPosition.field.tag_head.field.head = 0x5577;
//    tagSetStartPosition.field.tag_head.field.cmd = 0x01;
//    tagSetStartPosition.field.tag_head.field.index = 0x03;
//    /******************获取位置信息：01****************************/
//    memset(tagGetPosition.tagPara,0,sizeof(tagGetPosition.tagPara));
//    tagGetPosition.field.tag_head.field.head = 0x5577;
//    tagGetPosition.field.tag_head.field.cmd = 0x02;
//    tagGetPosition.field.tag_head.field.index = 0x01;
//    /******************获取速度信息：02****************************/
//    memset(tagGetPosition.tagPara,0,sizeof(tagGetPosition.tagPara));
//    tagGetPosition.field.tag_head.field.head = 0x5577;
//    tagGetPosition.field.tag_head.field.cmd = 0x02;
//    tagGetPosition.field.tag_head.field.index = 0x02;
}

void AGVTx::run()
{
  exec();
}

void AGVTx::recSendBufferSlot(QByteArray QReceivePara, ushort uSendCnt)
{
//    QList<uint8> a1;
//    ModBusSPara mPara;
//    for(int i = 0; i < uSendCnt; i++)
//    {
//        a1.append(QReceivePara[i]);
//    }
//    mPara.ParaStatus.field.Prior = 1;
//    mPara.ptagSaveData = a1;
//    MODBUSPara_FIFO(me,mPara);
    //判断为两条查询命令需要等待接收
    if(isAgvCheckCmd(QReceivePara))
    {
        if(bEnableSend) //查询参数需要回报
        {
            emit SendData_Sig(QReceivePara,uSendCnt);
            emit WaitRecieve_Sig(QReceivePara,uSendCnt);
            bEnableSend = false;
        }
    }
    else // 设置参数不需要回报
    {
        emit SendData_Sig(QReceivePara,uSendCnt);
    }
}
//*******************************************************************************
//* Function Name  :
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
void AGVTx::MODBUSPara_LIFO( MODBUSActive *me, ModBusSPara const e)
{
    if(me->SParaPool.nfree > 0)
    {
        ++me->SParaPool.tailSe;
        if (me->SParaPool.tailSe == me->SParaPool.end)                 /* need to wrap the tail? */
        {
            me->SParaPool.tailSe = 0;                        /* wrap around */
        }
//        me->RingBuffer[me->SParaPool.tailSe] = e;      /* buffer the old front evt */
        --me->SParaPool.nfree;                        /* update number of free events */
    }
    else
    {
        ;//over handle
    }
}


//*******************************************************************************
//* Function Name  :
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
uchar AGVTx::SendBitBufferDistill(uchar *ucBitBufferPara)
{
    uchar ucReturnBit;
    //
    if(*ucBitBufferPara & 0xF0)
        ucReturnBit = (log2Lkup[*ucBitBufferPara >> 4] - 1) + 4;   //
    else
        ucReturnBit = log2Lkup[*ucBitBufferPara] - 1;
    //
    *ucBitBufferPara &= g_ucInvBitMask[ucReturnBit];

    return(ucReturnBit);
}
//*******************************************************************************
//* Function Name  :
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
ModBusSPara AGVTx::MODBUSPara_Get(MODBUSActive *me)
{
    ModBusSPara  e={0};
    if(me->SParaPool.nfree != me->SParaPool.end)
    {
//        e =me->RingBuffer[me->SParaPool.tailSe];
        if(me->SParaPool.tailSe == 0)
        {
            me->SParaPool.tailSe = me->SParaPool.end;
        }
        --me->SParaPool.tailSe;
        ++me->SParaPool.nfree;
    }
    return (e);
}
//*******************************************************************************
//* Function Name  :
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
void AGVTx::MODBUSPara_FIFO(MODBUSActive *me,ModBusSPara e)
{
    me->SParaPool.nfree > 0;
    if(me->SParaPool.nfree > 0)
    {
        me->SParaPool.ring[me->SParaPool.head] = e;/*insert Spara to the buffer*/
        if(me->SParaPool.head == 0) /*need to wrap the head?*/
        {
            me->SParaPool.head = me->SParaPool.end;  /*wrap around*/
        }
        --me->SParaPool.head;
        --me->SParaPool.nfree;/*updata number of free Spara*/
    }
    else
    {
        ;// over handle
    }
}
//*******************************************************************************
//* Function Name  :
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
bool AGVTx::MODBUS_GetFIFOFree(void)
{
    if(me->SParaPool.nfree >0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//*******************************************************************************
//* Function Name  :
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
void AGVTx::AGV_EnableTxSend_Slot(bool bEnable)
{
    bEnableSend = bEnable;
}

//*******************************************************************************
//* Function Name  :
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
bool AGVTx::isAgvCheckCmd(QByteArray QReceivePara)
{
    TagheadCmd tagCmd;
    int iLength;
    iLength = sizeof(tagCmd.tagheadCmd);
    for(int i = 0; i < iLength ; i++ )
    {
        tagCmd.tagheadCmd[i] = QReceivePara.at(i);
    }
    if(tagCmd.field.cmd == 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}




