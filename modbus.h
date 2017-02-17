#ifndef MODBUS_H
#define MODBUS_H
#include <QObject>
#include "paratypedef.h"

//========================================================================================
// 常量定义
//========================================================================================

// 串口通信状态
#define SCI_STATE_INIT          0                                               // 初始化态
#define SCI_STATE_IDLE          1                                               // 空闲态
#define SCI_STATE_SEND          2                                               // 发送态
#define SCI_STATE_RECIV         3                                               // 接收态

// 定时器及时间常数
#define TIMER_SWITCH_OFF        0                                               // 定时器关
#define TIMER_SWITCH_ON         1                                               // 定时器开
#define TIMER_CHECK_PERIOD      62                                              // 定时器查看周期us

// 返回参数值最大字节数
#define MB_RETURN_MAX_CNT       128

#define SCI_BAUD                19200                                           // 串口波特率

#define TIMER15_CNT             2   //帧间隔1.5字符计数器
#define TIMER35_CNT             5   //帧间隔3.5字符计数器


// 用于位操作的功能码
#define MB_FUNCTION_REAND_COILS                 0x01                            // 读线圈状态
#define MB_FUNCTION_REAND_DISCRETE_INPUTS       0x02                            // 读离散输入开关状态
#define MB_FUNCTION_WRITE_SINGLE_COILS          0x05                            // 强制单个线圈
#define MB_FUNCTION_WRITE_MULTIPLE_COILS        0x0F                            // 强制多个线圈

// 用于16位寄存器操作的功能码
#define MB_FUNCTION_REAND_HOLDING_REGS          3                            // 读保持寄存器
#define MB_FUNCTION_REAND_INPUT_REGS            0x04                            // 读保输入存器
#define MB_FUNCTION_WRITE_SINGLE_REGS           0x06                            // 写数据到单个寄存器
#define MB_FUNCTION_WRITE_DIAGNOSTICS           0x08                            // 诊断
#define MB_FUNCTION_WRITE_MULTIPLE_REGS         0x10                            // 写数据到多个寄存器
#define MB_FUNCTION_REANDWRITE_MULTIPLE_REGS    0x17                            // 读写多个寄存器


// 帧检测结果
#define MB_FRAME_OK                             0x00                            // 帧正常
#define MB_FRAME_CRC_ERR                        0x01                            // 帧异常:CRC校验错误
#define MB_FRAME_LENGTH_ERR                     0x02                            // 帧异常:帧长度不正确

// 帧处理结果
#define MB_FRAME_SUCCESS                        0x00                            // 成功
#define MB_FRAME_ILLEGAL_FUNCTION               0x01                            // 无效的功能码
#define MB_FRAME_ILLEGAL_DATAADDRESS            0x02                            // 无效的数据地址
#define MB_FRAME_ILLEGAL_DATAVALUE              0x03                            // 无效数据值
#define MB_FRAME_ILLEGAL_SLAVEFAILURE           0x04                            // 从设备故障
#define MB_FRAME_CRC_OR_lENGTH                  0x11                            // CRC错误

// 各类数据的映射地址域
#define MB_INPUT_REGS_STAR_ADDRESS              1000                           // 输入寄存器起始地址
#define MB_INPUT_REGS_END_ADDRESS               5852                           // 输入寄存器起始地址

#define MB_HOLDING_REGS_STAR_ADDRESS            0                            // 保持寄存器起始地址
#define MB_HOLDING_REGS_END_ADDRESS             0x90                            // 保持寄存器起始地址

// 最大操作个数
#define MB_BIT_OPERSITION_MAX_CNT               2000                            // 位操作最大个数
#define MB_REGS_OPERSITION_MAX_CNT              128                             // 寄存器操作最大个数

#define MB_ADU_MAX_SIZE                         140                            // ADU最大字节数
#define MODBUSRINGBUFFERLEN                     10

//========================================================================================
// 宏函数定义
//========================================================================================

//========================================================================================
// 数据结构定义
//========================================================================================
//SCI通讯协议
typedef struct
{
    int16   iSlaveAddr;     //从机地址
    int16   iBaud;          //波特率0: 9600/1: 19200/2: 38400/3:57600/4:115200
    int16   iVerifyMode;    //校验模式0:(N 8 1)/1:(E 8 1)/2:(O 8 1)/3:(N 8 2)/4:(E 8 2)/5:(O 8 2)
    int16   iAckDelay;      //应答延迟时间
    int16   iAckDelayCnter; //应答延迟时间计数器
    int16   iOutTime;       //超时时间
    int16   iOutTimeCnter;  //超时时间计数器
    int16   iFaultAct;      //通讯错误处理方式
}StructTypeSciProtocTag;


// SCI对象信息
typedef union
{
    struct
    {
        Uint16  CurrentState        : 4;            // bit0-3:当前状态
        Uint16  RecvFirstData       : 1;            // bit4:收到第一个数据
        Uint16  ReceivedData        : 1;            // bit5:收到数据
        Uint16  RecvFrameFinish     : 1;            // bit6:数据就收完成
        Uint16  UnpackFinish        : 1;            // bit7:解包完成

        Uint16  SendFrameReady      : 1;            // bit8:发送帧就绪
        Uint16  NeedBackFlag        : 1;            // bit9:允许回包
        Uint16  Reserv1             : 6;            // 保留

        Uint16  Reserv2             : 16;           // 保留2
    } field;
    quint32  Value;
} Tag_SciInformation;

// SCI发送帧结构信息
typedef struct
{
    Uint16  SlaveAddress;                           // 从机地址
    Uint16  Function;                               // 功能码
    Uint16  SubFunction;                            // 子功能码
    Uint16  ByteQty;                                // 返回参数值字节数
    Uint16  OperationAddress;                       // 操作地址
    Uint16  OperationQty;                           // 操作个数
    Uint16  TotalSendByteQty;                       // 发送字节总数
    Uint16  DataValue[MB_RETURN_MAX_CNT];           // 返回参数值
} Tag_SciSendFrame;

// SCI接收帧结构信息
typedef struct
{
    Uint16  SlaveAddress;                           // 从机地址
    Uint16  Function;                               // 功能码
    Uint16  SubFunction;                            // 子功能码
    Uint16  ReadAddress;                            // 读地址
    Uint16  WriteAddress;                           // 写地址
    Uint16  ReadQty;                                // 读个数
    Uint16  WriteQty;                               // 写个数
    Uint16  WriteByteQty;                           // 写字节数
    Uint16  TotalRecvByteQty;                       // 接收字节总数
    Uint16  DataValue[MB_RETURN_MAX_CNT];           // 写参数值
    Uint16  FrameStete;                             // 帧状态
} Tag_SciRecvFrame;

typedef struct
{
    Uint16  uiT15Max;                               // 1.5字符间隔时间
    Uint16  uiT35Max;                               // 3.5字符间隔时间
    Uint32  TimerCNT_15;                            // 字符间间隔定时计数: 1.5个字符
    Uint32  TimerCNT_35;                            // 帧间隔定时计数: 3.5个字符
    Uint16  TimerSwitch_15          : 1;            // 定时开关: 1:开; 0:关
    Uint16  TimerSwitch_35          : 1;            // 定时开关: 1:开; 0:关
    Uint16  TimerOutFlag_15         : 1;            // 定时溢出标志: 1:定时到; 0:计数中
    Uint16  TimerOutFlag_35         : 1;            // 定时溢出标志: 1:定时到; 0:计数中
    Uint16  reserv1                 : 12;           // 保留
} Tag_SciTimer;

typedef enum{ReadData = 0,WriteData = 1,WriteAnser} OperateState;
typedef union
{
    struct{
        uchar ucDataLow;		//
        uchar ucDataHigh;		//ｲ
    } field;
    quint16 iData;
} tag_Data;
enum{
    MODBUS_READ = 1,
    MODBUS_MODIFY = 2,
    MODBUS_WRITEANSWER = 4
};
typedef union paraMode
{
    struct{
        tag_Data index;
        tag_Data para;
    }filed;
    unsigned char ucData[4];
}Tag_paraMode;

typedef union ParaStruct{
    struct{

        tag_Data max;
        tag_Data min;
        tag_Data Para;
        tag_Data Addr;
    }filed;
    unsigned char iData[8];
}Tag_para;
uint const  g_iSendId[] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080};
uchar const   log2Lkup[] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};
ushort const   g_ucInvBitMask[] = {0xFFFE, 0xFFFD, 0xFFFB, 0xFFF7, 0xFFEF, 0xFFDF, 0xFFBF, 0xFF7F,
                                0xFEFF,0xFDFF,0xFBFF,0xF7FF,0xEFFF,0xDFFF,0xBFFF,0x7FFF};
uchar const g_ucBitMask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

typedef union
{
    struct
    {
        unsigned RFlag				:1;
        unsigned WFlag              :1;
        unsigned WAFlag             :1;     // write answer Flag
        unsigned Prior				:1;		//ﾓﾅﾏﾈｼｶ
        unsigned Reserved			:12;	//
    }field;
    ushort SParaStatus;
}Tag_ModBusParaStatus;
/*********MODBUS RING BUFFER PARA****************/
typedef struct MODBUSParaTag{
    Tag_ModBusParaStatus	ParaStatus;				//
    QList<uint8>	ptagSaveData;			//
}ModBusSPara;
/********Ring Buffer of CAN TX****************/
typedef struct MODBUSRingBuffTag{
    QList<ModBusSPara> ring;
    uchar    end;
    uchar    head;
    uchar    tailSe;
    uchar    nfree;
    uchar    nMin;
}ModBusRingBuff;// wrap-around
typedef struct MODBUSTag {


    uchar ucReceiveLength;				//
    uchar ucSendLength;					//
    uchar ucReceiveIndex;               //
    uchar ucSendIndex;                  //
    uchar ucParaType;                   //
    uchar ucModbusErroCn;               //MODBUS
    uchar ucModbusFailCn; 				//MODBUS
    bool bUsartStart;					//USART
    ModBusRingBuff 	SParaPool;
//    ModBusSPara  	RingBuffer[MODBUSRINGBUFFERLEN];
    ModBusSPara		tag_MODBUSRecPara;								//BUFFER
    ModBusSPara 	tag_DistillPara;								//BUFFER
    ModBusSPara     tag_JogPara;                                    //
    uchar m_charPosSend;
    tag_Data Crc;
    QByteArray qSendDataArry;
} MODBUSActive;
#ifdef _MODBUS_C
    #define PROTO
#else   // 外部引用接口声明
    #define PROTO   extern
#endif  // end of #ifdef _THETA_C


PROTO Uint16 Sub_CalCRC(QList<uint8> CrcPointer, Uint16 Length);
PROTO Uint16 Sub_CalCRC(QByteArray CrcPointer, Uint16 Length);
PROTO Uint16 Sub_CalCRC(Uint16 *CrcPointer, Uint16 Length);
PROTO bool isValidCmd(Uint16 uCmd);
PROTO bool MODBUS_GetFIFOFree(void);
PROTO void MODBUSPara_LIFO( MODBUSActive *me, ModBusSPara const e);
PROTO void MODBUSPara_FIFO(MODBUSActive *me,ModBusSPara e);
PROTO ModBusSPara MODBUSPara_Get(MODBUSActive *me);
//PROTO void initLedCtrPara(void);
#undef PROTO
#endif // MODBUS_H
