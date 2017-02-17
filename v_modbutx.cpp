#include "v_modbutx.h"

v_modbutx::v_modbutx(QObject *parent):QThread(parent)
{
    //=============================================
    //initial ring buffer
    //=============================================
    me = new MODBUSActive();
    me->SParaPool.end = MODBUSRINGBUFFERLEN;
    me->SParaPool.head = 0;
    me->SParaPool.tailSe = 0;
    me->SParaPool.nfree = MODBUSRINGBUFFERLEN;
    me->SParaPool.nMin = MODBUSRINGBUFFERLEN;
    for(int i = 0; i < MODBUSRINGBUFFERLEN ; i++)
    {
        me->SParaPool.ring.append(me->tag_MODBUSRecPara);
    }
    me->ucReceiveIndex = 0;
    me->ucSendIndex = 0;
    me->ucModbusErroCn = 0;
//    bEnableSend = true;
}

void v_modbutx::run()
{

    exec();
}

void v_modbutx::recSendBufferSlot(QByteArray QReceivePara, ushort uSendCnt)
{
    QList<uint8> a1;
    ModBusSPara mPara;
    for(int i = 0; i < uSendCnt; i++)
    {
        a1.append(QReceivePara[i]);
    }
    mPara.ParaStatus.field.Prior = 1;
    mPara.ptagSaveData = a1;
    MODBUSPara_FIFO(me,mPara);

    emit SendData_Sig(QReceivePara,uSendCnt);
}

//*******************************************************************************
//* Function Name  :
//* Description    :
//* Input          :
//* output         :
//* return         :
//********************************************************************************/
void v_modbutx::MODBUSPara_LIFO( MODBUSActive *me, ModBusSPara const e)
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
uchar v_modbutx::SendBitBufferDistill(uchar *ucBitBufferPara)
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
ModBusSPara v_modbutx::MODBUSPara_Get(MODBUSActive *me)
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
void v_modbutx::MODBUSPara_FIFO(MODBUSActive *me,ModBusSPara e)
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
bool v_modbutx::MODBUS_GetFIFOFree(void)
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

