#ifndef AGVPARASTRUCT_H
#define AGVPARASTRUCT_H
#include <QList>
#include "paratypedef.h"
//========================================================================================
// 常量定义
//========================================================================================
#define AGVSET_LENGTH 28
#define AGVMAP_PICTURE_P 20.f

//========================================================================================
// 数据结构定义
//========================================================================================
union FData
{
    float d;
    unsigned char data[4];
};
typedef union  Position
{
    struct strPosition{
        FData x;
        FData y;
        FData A;
        FData w;
    }filed;
    unsigned char data[16];
}Tag_position;

typedef union nPosition
{
    struct strNPosition{
        FData x;
        FData y;
        FData Omega;
    }filed;
    unsigned char data[12];
}Tag_Nposition;

union tagUint16
{
    quint16 d;
    unsigned char data[2];
};
typedef union taghead{
    struct strhead{
        unsigned char first;
        unsigned char secnod;
    }filed;
    tagUint16 data;
}Tag_head;

typedef union headCmd{
    struct StrheadCmd{
        Tag_head head;
        unsigned char cmd;
        unsigned char index;
    }field;
    unsigned char tagheadCmd[4];
}TagheadCmd;

typedef union setMotion{
   struct ParaAnaly{
       TagheadCmd tag_head;
       FData forword;
       FData rotation;
       unsigned char reserved[16];
   }field;
    unsigned char tagPara[28];
}Tag_setMotion;//

typedef union setPosition{
    struct ParaAnaly{
        TagheadCmd tag_head;
        FData x;
        FData y;
        FData A;
        FData w;
        unsigned char reserved[8];
    }field;
    unsigned char tagPara[28];
}Tag_setPosition;

typedef union getInfo{
    struct ParaPackge{
        TagheadCmd tag_head;
        unsigned char reserved[24];
    }field;
    unsigned char tagPara[28];

}Tag_getInfo;

typedef union backPosition{
    struct ParaAnaly{
        TagheadCmd tag_head;
        FData x;
        FData y;
        FData A;
        FData w;
        unsigned char reserved[10];
    }field;
    unsigned char tagPara[30];
}Tag_backPosition;

typedef union backSpdAndSensor{
    struct ParaAnaly{
        TagheadCmd tag_head;
        FData forwardSpd;
        unsigned char reserved[4];
        FData rotationSpd;
        unsigned char reserved1[1];
        tagUint16 leftFwdUltrasonic;
        tagUint16 rightFwdUltrasonic;
        tagUint16 leftBckUltrasonic;
        tagUint16 rightBckUltrasonic;
        unsigned char leftCollision;
        unsigned char rightCollision;
        unsigned char leftWheelNoContact;
        unsigned char rightWheelNoContact;
        unsigned char reserved2[1];
    }field;
    unsigned char tagPara[30];
}Tag_backSpdAndSensor;
typedef union backAgvState{

    struct ParaAnaly{
        TagheadCmd tag_head;
        unsigned char state;
        unsigned char reserved2[25];
    }field;
    unsigned char tagPara[30];
}Tag_backAgvState;

enum AgvState{
    StartExecution = 0,
    SuccessArrival,
    FailedArrival,
    MissionCancle,
    LostAim
};
typedef struct strLabel{
    QString name;
    int x;
    int y;
    float Rotation;
    int xScale;
    int yScale;
}Str_Label;

typedef struct strfixLabel{
    QString name;
    int iPicX;
    int iPicY;
    float fMapX;
    float fMapY;
    float fMapTheta;
    float fMapOmega;
    int ilabelWidth;
    int ilabelHeight;
}Str_FixLabel;
//================================
//
//===============================
typedef struct strLabelTest{
    QString name;
    float x;
    float y;
    float A;
    float W;
    int voiceIndex;
}Str_LabelTest;

typedef struct strVoiceTest{
    QString name;
    int voiceIndex;
}Str_voiceTest;
#ifdef _AGVPARA_C
    #define PROTO
#else   // 外部引用接口声明
    #define PROTO   extern
#endif  // end of #ifdef _AGVPARA_C
//========================================================================
//变量定义
//========================================================================
PROTO Tag_backAgvState tagAgvBackAgvState;
PROTO Tag_backPosition tagAgvBackPosition;
PROTO Tag_backSpdAndSensor tagAgvBackSpdAndSensor;
PROTO QList<Tag_Nposition> tagAgvNormalAimposition; //设定导航序列

PROTO QList<Tag_position>  tagVoiceGuideposition;   //设定地址序列
PROTO QList<Str_Label>  tagMapLabelList;            //设定目标地址序列
PROTO QList<Str_FixLabel> tagFixMapLabelList;       //设定固定目标地址序列
PROTO QList<Str_FixLabel> tagPollingMapLabelList;   //设定巡检目标地址序列

PROTO Uint16 ushortAgvState;             //AGV当前状态
PROTO Uint16 uAlreadySendAGVPositionCnt; //已经发送AGV目标序列记数
PROTO Tag_position tagAgvOldSendPosition;

PROTO Tag_position tagAgvFinalAimPosition; //发送目的地位置
PROTO Tag_position tagStartPosition;     //机器人初始位置
PROTO Uint16 uAgvIsNavigation;
PROTO float agv_AxisoffsetX;
PROTO float agv_AxisoffsetY;
PROTO QString sMapfileUrl;
//===========================================
//TEST
//===========================================
PROTO QList<Str_LabelTest> m_strLabelTestList;
//test
PROTO QList<Str_voiceTest> tagVoiceTest;
PROTO QList<Tag_position> tagCheckTEst;
//========================================================================
//函数定义
//========================================================================

PROTO void agvMapPara_Init();//
PROTO void agvMapPara_Statehandle();
PROTO bool dbConnect(QString dbname);


//=======================================================================
//数据库操作
//=======================================================================
//PROTO void dbInitRead();
PROTO bool  dbSetMapUrl(QString URL);
PROTO QString dbReadMapUrl();
PROTO bool dbSetOffset(float xoffset,float yoffset);
PROTO float dbReadXoffset();
PROTO float dbReadYoffset();
PROTO bool dbSetMapLabel(QString name,int x,int y,float Rotation,int xScale,int yScale);
PROTO QString dbReadLabelName();
PROTO Str_Label dbReadMapLabel(int id);
PROTO int dbReadMapLabelMax();
PROTO bool dbLabelDeleteAll();
PROTO bool dbLabelDeleteOne(int x,int y);
PROTO bool readLabePosition(); //读标签到缓存
//保存初始位置
PROTO bool dbSetStartPosition(float x,float y,float w,float a);
PROTO Tag_position dbReadStartPosition();
PROTO void readStartPosition();
//==========================================
//固定标签
//==========================================
//保存固定标签位置
PROTO bool dbSetfixMapLabel(QString name,int iPicX,int iPicY,float fMapx,float fMapy,float omega,float theta,
                            int width,int height);
PROTO Str_FixLabel dbReadFixMapLabel(int id);
PROTO bool readFixLabelPosition();//读固定标签
PROTO int dbReadFixMapLabelMax();
PROTO bool dbFixLabelDeleteAll();
//=========================================
//巡检标签
//=========================================
//保存巡检标签
PROTO bool dbSetPollingMapLabel(QString name,int iPicX,int iPicY,float fMapx,float fMapy,float omega,float theta,
                                int width,int height);
PROTO Str_FixLabel dbReadPollingMapLabel(int id);
PROTO bool readPollingLabelPosition();//读固定标签
PROTO int dbReadPollingMapLabelMax();
PROTO bool dbPollingLabelDeleteAll();
#undef PROTO
#endif // AGVPARASTRUCT_H
