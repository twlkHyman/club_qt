#include "v_modbusrx.h"

v_modbusrx::v_modbusrx(QObject *parent):QThread(parent)
{
    gSciSendFrame = new Tag_SciSendFrame();

}

void v_modbusrx::receiveDataSlot(QByteArray buffer,int cnt)
{
    QByteArray qSendBuffer;
    char* str = buffer.data();

//    Uint16 FrameCheckState		= 0;
//    memcpy(m_receivedBuffer,buffer,cnt);
//    buffer.toUShort(0,16);
    for(int i= 0; i < cnt; i++)
    {
        m_receivedBuffer[i] = (uint8)buffer[i];
    }
    gSciRecvFrame.TotalRecvByteQty = cnt;
    SciUnpack(gSciSendFrame,(Uint16*)&m_receivedBuffer);
    SciPack(gSciSendFrame,m_sendBuffer);
    if(gSciInformation.field.SendFrameReady)
    {
        gSciInformation.field.SendFrameReady = 0;
        qSendBuffer.resize(gSciSendFrame->TotalSendByteQty);
        for(int i = 0; i< gSciSendFrame->TotalSendByteQty; i++)
        {
            qSendBuffer[i] = (uint8_t)m_sendBuffer[i] & 0xFF;
        }
        emit SendBuffer_Sig(qSendBuffer,gSciSendFrame->TotalSendByteQty);
    }
}


//########################################################################################
// Function Name:	SciUnpack
// Version:			V1.0
// Input:			SciSendFrame, RecvBuff
// Output:			none
// Description:		接收完成后,将接受缓存中的数据解析到对应的结构中
//########################################################################################
void v_modbusrx::SciUnpack(Tag_SciSendFrame* SciSendFrame, Uint16* RecvBuff)
{
    Uint16 FrameCheckState		= 0;
    Uint16 FrameProcessState	= 0;
    Tag_SciRecvFrame RecvFrame;


    FrameCheckState = SciFrameCheck(RecvBuff);

    SciSendFrame->SlaveAddress	= RecvBuff[0];
    SciSendFrame->Function		= RecvBuff[1];
    if ((FrameCheckState == MB_FRAME_OK)
        || (FrameCheckState == MB_FRAME_CRC_ERR))
    {
        if(FrameCheckState == MB_FRAME_CRC_ERR)
        {
            FrameProcessState = MB_FRAME_CRC_OR_lENGTH;
        }
        else
        {
            switch ((uint8)RecvBuff[1])
            {
            case MB_FUNCTION_REAND_HOLDING_REGS:
//                FrameProcessState = ReadHoldingRegisters(&RecvFrame, RecvBuff);
                break;
            case MB_FUNCTION_REAND_INPUT_REGS:
//                FrameProcessState = ReadInputRegisters(&RecvFrame, RecvBuff);
                ;;
                ;;
                ;;
                FrameProcessState = MB_FRAME_CRC_OR_lENGTH;
                break;
            case MB_FUNCTION_WRITE_SINGLE_REGS:
                FrameProcessState = WriteSingleRegister(&RecvFrame, RecvBuff);
                break;
            case MB_FUNCTION_WRITE_MULTIPLE_REGS:
                FrameProcessState = WriteMultipleRegisters(&RecvFrame, RecvBuff);
                break;
            default:
//                FrameProcessState = MB_FRAME_ILLEGAL_FUNCTION;			// 功能码无效
                break;
            }
        }
        if (FrameProcessState == MB_FRAME_SUCCESS)
        {
            //处理请求
            FrameProcessState = SciProcessRequest(SciSendFrame, RecvFrame);
            gSciRecvFrame.FrameStete = FrameProcessState;
            gSciInformation.field.UnpackFinish = 1;
        }
        else
        {
           gSciInformation.field.UnpackFinish = 0;
        }

    }
    else if (FrameCheckState == MB_FRAME_LENGTH_ERR)
    {
        //帧长度不一致, 不回复任何消息, 清空接收缓存
        gSciInformation.Value = 0;
        gSciRecvFrame.TotalRecvByteQty	= 0;

//        gSciTimer.TimerCNT_15 = 0;
//        gSciTimer.TimerCNT_35 = 0;
//        gSciTimer.TimerOutFlag_15 = 0;
//        gSciTimer.TimerOutFlag_35 = 0;
//        gSciTimer.TimerSwitch_15 = TIMER_SWITCH_OFF;
//        gSciTimer.TimerSwitch_35 = TIMER_SWITCH_ON;
//        gSciInformation.field.CurrentState = SCI_STATE_INIT;
    }
}
//########################################################################################
// Function Name:	SciFrameCheck
// Version:			V1.0
// Input:			Recvbuff
// Output:			TempFrameState
// Description:		帧检测; CRC和帧长度
//########################################################################################
Uint16 v_modbusrx::SciFrameCheck(Uint16* Recvbuff)
{
    Uint16 TempCrcData		= 0;
    Uint16 TempFrameState	= 0;
    Uint16 TempFrameLength	= 0;
    Uint16 TempCrcHigh      = 0;
    Uint16 TempCrcLow       = 0;

    switch (Recvbuff[1])
    {
    case MB_FUNCTION_REAND_COILS:
    case MB_FUNCTION_REAND_DISCRETE_INPUTS:
    case MB_FUNCTION_REAND_HOLDING_REGS:
    case MB_FUNCTION_WRITE_SINGLE_COILS:
    case MB_FUNCTION_WRITE_SINGLE_REGS:
        TempFrameLength = 8;
        break;
    case MB_FUNCTION_REAND_INPUT_REGS:				// 为了避免03号节点误接收其它从站返回的读指令
        TempFrameLength = 100;
        break;
    case MB_FUNCTION_WRITE_MULTIPLE_COILS:
    case MB_FUNCTION_WRITE_MULTIPLE_REGS:
        TempFrameLength = Recvbuff[6] + 9;
        break;
    case MB_FUNCTION_REANDWRITE_MULTIPLE_REGS:
        TempFrameLength = Recvbuff[10] + 13;
        break;
    default:
        break;
    }

    if ((gSciRecvFrame.TotalRecvByteQty == TempFrameLength) && (TempFrameLength > 2))
    {
        //CRC计算
        TempCrcData = Sub_CalCRC(Recvbuff, (TempFrameLength - 2));

        TempCrcHigh = (Uint16)(Recvbuff[TempFrameLength - 1] * 256);
        TempCrcLow = (Recvbuff[TempFrameLength -2]);
        //移位操作有问题，乘以256
        if(TempCrcData == (Uint16)(Recvbuff[TempFrameLength - 1] * 256 + Recvbuff[TempFrameLength - 2]))
        {
            TempFrameState = MB_FRAME_OK;
        }
        else
        {
            TempFrameState = MB_FRAME_CRC_ERR;
        }
    }
    else
    {
        TempFrameState = MB_FRAME_LENGTH_ERR;
    }

    return TempFrameState;
}

//########################################################################################
// Function Name:	WriteSingleRegister
// Version:			V1.0
// Input:			RecvFrame, Recvbuff
// Output:			TempReturnState
// Description:		写单个
//########################################################################################
Uint16 v_modbusrx::WriteSingleRegister(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff)
{
    Uint16 TempReturnState = 0;												//返回状态

    RecvFrame->DataValue[0] = (Recvbuff[5] + Recvbuff[4] * 256);

//	if ((0 <= RecvFrame->DataValue[0]) && (RecvFrame->DataValue[0] < 0xFFFF))
    {
        RecvFrame->WriteAddress = (Recvbuff[3] + Recvbuff[2] * 256);

        if ((MB_HOLDING_REGS_STAR_ADDRESS <= RecvFrame->WriteAddress)
            && (RecvFrame->WriteAddress < MB_HOLDING_REGS_END_ADDRESS))
        {
            TempReturnState = MB_FRAME_SUCCESS;
        }
        else
        {
            TempReturnState = MB_FRAME_ILLEGAL_DATAADDRESS;					//数据地址无效
        }
    }
    if(TempReturnState == MB_FRAME_SUCCESS)
    {

    }
    return TempReturnState;
}
//########################################################################################
// Function Name:	WriteMultipleRegisters
// Version:			V1.0
// Input:			RecvFrame, Recvbuff
// Output:			TempReturnState
// Description:		写多个寄存器
//########################################################################################
Uint16 v_modbusrx::WriteMultipleRegisters(Tag_SciRecvFrame* RecvFrame, Uint16* Recvbuff)
{
    Uint16 TempReturnState = 0;												//返回状态
    Uint16 i = 0;

    RecvFrame->WriteQty = (Recvbuff[5] + Recvbuff[4] * 256);
    RecvFrame->WriteByteQty= RecvFrame->WriteQty << 1;

    if (((1 <= RecvFrame->WriteQty) && (RecvFrame->WriteQty <= MB_REGS_OPERSITION_MAX_CNT))
        && (RecvFrame->WriteByteQty == Recvbuff[6]))
    {
        RecvFrame->WriteAddress = (Recvbuff[3] + Recvbuff[2] * 256);

        if (((MB_HOLDING_REGS_STAR_ADDRESS <= RecvFrame->WriteAddress)  && (RecvFrame->WriteAddress <= MB_HOLDING_REGS_END_ADDRESS))
            && ((RecvFrame->WriteAddress + RecvFrame->WriteQty) <= (MB_HOLDING_REGS_END_ADDRESS + 1)))
        {
            for(i = 0; i < RecvFrame->WriteQty; i++)						//提取写参数值
            {
                RecvFrame->DataValue[i] = (Recvbuff[8 + i * 2] + Recvbuff[7 + i * 2] * 256);
            }

            TempReturnState = MB_FRAME_SUCCESS;
        }
        else
        {
            TempReturnState = MB_FRAME_ILLEGAL_DATAADDRESS;					//数据地址无效
        }
    }
    else
    {
        TempReturnState = MB_FRAME_ILLEGAL_DATAVALUE;						//数据值无效
    }
    return TempReturnState;
}
//########################################################################################
// Function Name:	WriteMultipleCoils
// Version:			V1.0
// Input:			SciSendFrame, RecvFrame
// Output:			TempReturnState
// Description:		处理请求
//########################################################################################
Uint16 v_modbusrx::SciProcessRequest(Tag_SciSendFrame* SciSendFrame, Tag_SciRecvFrame RecvFrame)
{
    int32 lReturnValue = 0;
    Uint16 TempReturnState = 0;											// 返回状态
    Uint16 i = 0;
    QVector <Uint16> qvSendByte(0);
    QByteArray cmd,para;
    int32 lParaTemp = 0;
    switch (SciSendFrame->Function)
    {
    case MB_FUNCTION_REAND_HOLDING_REGS:								// 读保持寄存器
    case MB_FUNCTION_REAND_INPUT_REGS:									// 读输入寄存器
        SciSendFrame->OperationQty = RecvFrame.ReadQty;
        SciSendFrame->ByteQty = RecvFrame.ReadQty << 1;

        if(RecvFrame.ReadQty == 1)
        {
//            lReturnValue = DataExchange(COMM_PROTOCOL_MODBUS, COMM_DATA_OUTPUT,(Uint32)RecvFrame.ReadAddress,
//                            &uiParaBitLength, (int64*)&lParaTemp);
//            SciSendFrame->DataValue[0] = LOBYTE(lParaTemp);

//            if ((lReturnValue == OD_VALUE_TOO_HIGH)
//                || (lReturnValue == OD_VALUE_TOO_LOW))
//            {
//                TempReturnState = MB_FRAME_ILLEGAL_DATAVALUE;
//            }
        }
        else
        {
//            for(iSciCnt = 0; iSciCnt < (RecvFrame.ReadQty >> 1); iSciCnt++)
//            {
//                lReturnValue = DataExchange(COMM_PROTOCOL_MODBUS, COMM_DATA_OUTPUT,(Uint32)RecvFrame.ReadAddress,
//                            &uiParaBitLength, (int64*)&lParaTemp);

//                if ((lReturnValue == OD_VALUE_TOO_HIGH)
//                || (lReturnValue == OD_VALUE_TOO_LOW))
//                {
//                    TempReturnState = MB_FRAME_ILLEGAL_DATAVALUE;
//                }

//                //每次处理一个32位数据
//                SciSendFrame->DataValue[iSciCnt * 2] 		= LOBYTE(lParaTemp);
//                SciSendFrame->DataValue[iSciCnt * 2 + 1] 	= HIBYTE(lParaTemp);

//                //下一个参数通讯地址
//                RecvFrame.ReadAddress += 2;
//            }
        }
        break;

    case MB_FUNCTION_WRITE_SINGLE_REGS:									// 写单个寄存器
        SciSendFrame->OperationAddress = RecvFrame.WriteAddress;

        lParaTemp = (int32)RecvFrame.DataValue[0];
        SciSendFrame->DataValue[0] = (quint16)RecvFrame.DataValue[0];
        emitReceiveCmdOver(RecvFrame.WriteAddress , RecvFrame.DataValue[0]);
//        lReturnValue = DataExchange(COMM_PROTOCOL_MODBUS, COMM_DATA_INPUT,(Uint32)RecvFrame.WriteAddress,
//                    &uiParaBitLength, (int64*)&lParaTemp);

//        if ((lReturnValue == OD_VALUE_TOO_HIGH)
//                || (lReturnValue == OD_VALUE_TOO_LOW))
//        {
//            TempReturnState = MB_FRAME_ILLEGAL_DATAVALUE;
//        }
        break;

    case MB_FUNCTION_WRITE_MULTIPLE_REGS:								// 写多个寄存器
        SciSendFrame->OperationAddress	= RecvFrame.WriteAddress;
        SciSendFrame->OperationQty		= RecvFrame.WriteQty;
        for(i = 0; i < RecvFrame.WriteQty ; i++)
        {
            //每次处理一个16位数据
            lParaTemp = (Uint16)RecvFrame.DataValue[i] ;
            qvSendByte.append(RecvFrame.DataValue[i]);          //文字处理
//            lReturnValue = DataExchange(COMM_PROTOCOL_MODBUS, COMM_DATA_INPUT,(Uint32)RecvFrame.WriteAddress,
//                        &uiParaBitLength, (int64*)&lParaTemp);

//            if ((lReturnValue == OD_VALUE_TOO_HIGH)
//                || (lReturnValue == OD_VALUE_TOO_LOW))
//            {
//                TempReturnState = MB_FRAME_ILLEGAL_DATAVALUE;
//            }

            //下一个参数通讯地址
            RecvFrame.WriteAddress += 1;
        }

        emit SendVoice_Sig(qvSendByte,RecvFrame.WriteQty );
        break;
    default:
        break;
    }

    return TempReturnState;
}
//########################################################################################
// Function Name:	WriteMultipleCoils
// Version:			V1.0
// Input:			SendFrame, SendBuff
// Output:			none
// Description:		数据打包
//########################################################################################
void v_modbusrx::SciPack(Tag_SciSendFrame *SendFrame, Uint16* SendBuff)
{
    Uint16 FrameProcessState	= 0;
    Uint16 SendByteQty			= 0;
    Uint16 CrcData				= 0;
    Uint16 i					= 0;
    if (gSciInformation.field.UnpackFinish)
    {
        gSciInformation.field.UnpackFinish = 0;

        FrameProcessState = gSciRecvFrame.FrameStete;

        //广播帧处理
        if (SendFrame->SlaveAddress == 0)
        {
            if (FrameProcessState == MB_FRAME_SUCCESS)
            {
                //广播帧正常，不回复，转到接收态
//                gSciInformation.field.CurrentState = SCI_STATE_IDLE;
                gSciRecvFrame.TotalRecvByteQty		= 0;
                return;
            }
            else
            {
                //广播帧出错
                SendBuff[SendByteQty++] = SendFrame->SlaveAddress;
                SendBuff[SendByteQty++] = SendFrame->Function + 0x80;
                SendBuff[SendByteQty++] = FrameProcessState;
            }
        }
        else
        {
            if (FrameProcessState == MB_FRAME_SUCCESS)
            {
                SendBuff[SendByteQty++] = SendFrame->SlaveAddress;
                SendBuff[SendByteQty++] = SendFrame->Function;

                // 数据字节数:读取的数据字节数
                switch (SendFrame->Function)
                {
                case MB_FUNCTION_REAND_COILS:
                case MB_FUNCTION_REAND_DISCRETE_INPUTS:
                case MB_FUNCTION_REAND_HOLDING_REGS:
                case MB_FUNCTION_REAND_INPUT_REGS:
                case MB_FUNCTION_REANDWRITE_MULTIPLE_REGS:
                    SendBuff[SendByteQty++] = SendFrame->ByteQty;
                    break;
                default:
                    break;
                }

                // 操作地址
                switch(SendFrame->Function)
                {
                case MB_FUNCTION_WRITE_SINGLE_COILS:
                case MB_FUNCTION_WRITE_SINGLE_REGS:
                case MB_FUNCTION_WRITE_MULTIPLE_COILS:
                case MB_FUNCTION_WRITE_MULTIPLE_REGS:
                    SendBuff[SendByteQty++] = (SendFrame->OperationAddress >> 8 & 0xFF);
                    SendBuff[SendByteQty++] = (SendFrame->OperationAddress & 0xFF);
                    break;
                default:
                    break;
                }

                // 操作对象个数,如: 10个线圈、5个寄存器
                switch(SendFrame->Function)
                {
                case MB_FUNCTION_WRITE_MULTIPLE_COILS:
                case MB_FUNCTION_WRITE_MULTIPLE_REGS:
                    SendBuff[SendByteQty++] = (SendFrame->OperationQty >> 8 & 0xFF);
                    SendBuff[SendByteQty++] = (SendFrame->OperationQty & 0xFF);
                    break;
                default:
                    break;
                }

                // 操作数据
                switch(SendFrame->Function)
                {
                case MB_FUNCTION_REAND_COILS:
                case MB_FUNCTION_REAND_DISCRETE_INPUTS:
                    for (i = 0; i < SendFrame->ByteQty; i++)
                    {
                        SendBuff[SendByteQty++] = SendFrame->DataValue[i];
                    }
                    break;
                case MB_FUNCTION_REAND_HOLDING_REGS:
                case MB_FUNCTION_REAND_INPUT_REGS:
                case MB_FUNCTION_REANDWRITE_MULTIPLE_REGS:
                    for (i = 0; i <  SendFrame->OperationQty; i++)
                    {
                        SendBuff[SendByteQty++] = (SendFrame->DataValue[i] >> 8 & 0xFF);
                        SendBuff[SendByteQty++] = (SendFrame->DataValue[i] & 0xFF);
                    }
                    break;
                case MB_FUNCTION_WRITE_SINGLE_COILS:
                case MB_FUNCTION_WRITE_SINGLE_REGS:
                    SendBuff[SendByteQty++] = (SendFrame->DataValue[0] >> 8 & 0xFF);
                    SendBuff[SendByteQty++] = (SendFrame->DataValue[0] & 0xFF);
                    break;
                default:
                    break;
                }
            }
            else
            {
                SendBuff[SendByteQty++] = SendFrame->SlaveAddress;
                SendBuff[SendByteQty++] = SendFrame->Function + 0x80;
                SendBuff[SendByteQty++] = FrameProcessState;
            }
        }

        // 计算CRC
        CrcData = Sub_CalCRC(SendBuff, SendByteQty);
        SendBuff[SendByteQty++] = CrcData & 0x0FF;
        SendBuff[SendByteQty++] = (CrcData >> 8) & 0x0FF;

        // 出口
        gSciSendFrame->TotalSendByteQty = SendByteQty;

        // 应该清空接收帧和接收缓
        gSciRecvFrame.TotalRecvByteQty	= 0;
        gSciRecvFrame.FrameStete = 0;

        gSciInformation.field.SendFrameReady = 1;						// 发送帧准备好

    }
}

void v_modbusrx::emitReceiveCmdOver(Uint16 cmd,Uint16 para)
{
    QByteArray qCmd,qPara;
    tag_Data tagCmd,tagPara;
    tagCmd.iData = cmd;
    tagPara.iData = para;
    qCmd.append(tagCmd.field.ucDataHigh);
    qCmd.append(tagCmd.field.ucDataLow);
    qPara.append(tagPara.field.ucDataHigh);
    qPara.append(tagPara.field.ucDataLow);
    emit vrx_ReceiveCmdOver_Sig(qCmd,qPara);
}
