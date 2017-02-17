#include "voice.h"

voice::voice(QObject *parent) : QObject(parent)
{
    me = new MODBUSActive();
    m_vspcom = new SPcom();
//    m_vspcom->moveToThread(m_vspcom);
    m_vspcom->start();
    m_vmodbusRx = new v_modbusrx();
    m_vmodbusRx->moveToThread(m_vmodbusRx);
    m_vmodbusRx->start();
    m_vmodbusTx = new v_modbutx();
    m_vmodbusTx->moveToThread(m_vmodbusTx);
    m_vmodbusTx->start();
    connect(m_vmodbusTx,SIGNAL(SendData_Sig(QByteArray,ushort)),m_vspcom,SLOT(writeDataSlot(QByteArray,ushort)));
    connect(m_vspcom,SIGNAL(DataReciveOverSig(QByteArray,int)),m_vmodbusRx,SLOT(receiveDataSlot(QByteArray,int)));
    connect(m_vmodbusRx,SIGNAL(SendBuffer_Sig(QByteArray, ushort)),m_vmodbusTx,SLOT(recSendBufferSlot(QByteArray,ushort)));
    connect(m_vmodbusRx,SIGNAL(SendVoice_Sig(qVect16,ushort)),this,SLOT(recVoiceWordSlot(qVect16 ,ushort)));
    //在接收对象，接收完命令参数后发送出命令接收完消息，参数带命令和命令值
    connect(m_vmodbusRx,SIGNAL(vrx_ReceiveCmdOver_Sig(QByteArray,QByteArray)),this,SLOT(v_RecCmdSlot(QByteArray,QByteArray)));

    //TESTTESTTEST
    Str_voiceTest strVoice0;
    Str_voiceTest strVoice1;
    Str_voiceTest strVoice2;
    Str_voiceTest strVoice3;
    Str_voiceTest strVoice4;
    Str_voiceTest strVoice5;
    Str_voiceTest strVoice6;
    Str_voiceTest strVoice7;
    Str_voiceTest strVoice8;
    Str_voiceTest strVoice9;
    Str_voiceTest strVoice10;
    Str_voiceTest strVoice11;
    Str_voiceTest strVoice12;

    strVoice0.name = "stop";
    strVoice0.voiceIndex = 0;

    strVoice1.name = "bankintro";
    strVoice1.voiceIndex = 1;

    strVoice2.name = "ackaim";
    strVoice2.voiceIndex = 2;

    strVoice3.name = "followme";
    strVoice3.voiceIndex = 3;

    strVoice4.name = "arrival";
    strVoice4.voiceIndex = 4;

    strVoice5.name = "insertbankcard";
    strVoice5.voiceIndex = 5;

    strVoice6.name = "printorder";
    strVoice6.voiceIndex = 6;

    strVoice7.name = "printover";
    strVoice7.voiceIndex = 7;

    strVoice8.name = "selectservice";
    strVoice8.voiceIndex = 8;

    strVoice9.name = "selectcomservice";
    strVoice9.voiceIndex = 9;

    strVoice10.name = "selectprivate";
    strVoice10.voiceIndex = 10;

    strVoice11.name = "survey";
    strVoice11.voiceIndex = 11;

    strVoice12.name = "selectbankservice";
    strVoice12.voiceIndex = 12;

    tagVoiceTest.append(strVoice0);
    tagVoiceTest.append(strVoice1);
    tagVoiceTest.append(strVoice2);
    tagVoiceTest.append(strVoice3);
    tagVoiceTest.append(strVoice4);
    tagVoiceTest.append(strVoice5);
    tagVoiceTest.append(strVoice6);
    tagVoiceTest.append(strVoice7);
    tagVoiceTest.append(strVoice8);
    tagVoiceTest.append(strVoice9);
    tagVoiceTest.append(strVoice10);
    tagVoiceTest.append(strVoice11);
    tagVoiceTest.append(strVoice12);
}

void voice::setportName(const QString &name)
{
    m_vspcom->setPortName(name);
}

QString voice::portName()const
{
    return m_vspcom->PortName();
}

int voice::baudRate() const
{
    return m_vspcom->BaudRate();
}

void voice::setbaudRate(int baudRate)
{
    m_vspcom->setBaudRate(baudRate);
}

bool voice::isOpen()const
{
    return m_vspcom->isOpen();
}

bool voice::open()
{
    return m_vspcom->open();
}

void voice::close()
{
    m_vspcom->close();
}

bool voice::clear()
{

    return m_vspcom->clear();
}

int voice::writeData()
{
    char cBuffer[10]={0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A};
    return m_vspcom->writeData(cBuffer,10);
}

int voice::readData(char *buffer, int count, int timeout)
{
    return m_vspcom->readData(buffer,count,timeout);
}

int voice::write(char ch)
{
//    return m_spcom->write(ch);
    return 0;
}

void voice::recDataSlot(QByteArray buffer, int cnt)
{
    emit recOverSig(buffer,cnt);
}

void voice::recVoiceWordSlot(qVect16 qWord, ushort cnt)
{

    QString t1;
    foreach (const ushort h,qWord)
    {
        t1.append(h);
    }
    emit sendVoiceWordSig(t1);
}

void voice::testEmitCmd()
{
    QByteArray qSend,qPara;
    qSend.append(1);
    qSend.append(2);
    qSend.append(3);
    qPara.append(1);
    qPara.append(2);

    emit v_cmdReceiveOverSig(qSend,qPara);
}

void voice::v_RecCmdSlot(QByteArray cmd, QByteArray para)
{
    emit v_cmdReceiveOverSig(cmd,para);
}

//===========================================
//TEST
//============================================

void voice::v_VoicePlay(QString name)
{
    char cTestVoiceAddrHigh = 0;
    char cTestVoiceParaHigh = 0;
    if(name != "")
    {
        foreach (Str_voiceTest strvoicetest, tagVoiceTest) {
            if(strvoicetest.name == name)
            {
                QByteArray ucSendBuffer;
                ucSendBuffer.append(0x02);
                ucSendBuffer.append(MB_FUNCTION_WRITE_SINGLE_REGS);
                ucSendBuffer.append(cTestVoiceAddrHigh);
                ucSendBuffer.append(0x90);
                ucSendBuffer.append(cTestVoiceParaHigh);
                ucSendBuffer.append(strvoicetest.voiceIndex);

                me->Crc.iData = Sub_CalCRC(ucSendBuffer, ucSendBuffer.count());

                ucSendBuffer.append(me->Crc.field.ucDataLow);
                ucSendBuffer.append(me->Crc.field.ucDataHigh);
                m_vspcom->writeDataSlot(ucSendBuffer,ucSendBuffer.count());
                break;
            }
        }
    }
}
