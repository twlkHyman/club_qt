#ifndef AGVFLOATTOSTRING_H
#define AGVFLOATTOSTRING_H
#include "string.h"
union FData
{
    float d;
    unsigned char data[4];
};
#ifdef _AGVFLOATTOSTRING_C
    #define PROTO
#else   // 外部引用接口声明
    #define PROTO   extern
#endif  // end of #ifdef _AGVFLOATTOSTRING_C

PROTO  agvFloatToString(float fPara);
PROTO float agvStringToFloat(string sPara);
#endif // AGVFLOATTOSTRING_H
