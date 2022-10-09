#ifndef _HKTMRAWDATABUF_H
#define _HKTMRAWDATABUF_H

#include "GlobalMacros.h"

typedef struct
{
    char CDBPid[MAXPIDLEN];
    char StringValue[MAXCHARVALFORPID + 1];
    double RealValue;
    long long int TmCount;
    short int UpdateFrameId;
    unsigned char PidUpdateTime[10]; // YYYY MM DD HH MM SS MSC
} OneHkTmProcPidDef;

// One HkTm
typedef struct
{
    unsigned OneRawHkTmFrame[MAXWORDCOUNT];
    short int TimeStatus; // 1 -> New , 0 -> Old

} OneRawHkTmFrameDef;

/*
Following buffer is meant for a unique combination of
STN+STREAM+OBC
*/
typedef struct
{
    OneHkTmProcPidDef HkTmPrcOpBuf[IMXPID];
    OneRawHkTmFrameDef RawHkTmMasterFrame[MAXFRAMECOUNT];
    unsigned char RawDwlFrame[MAXWORDCOUNT];
    char ScId[6];
    char StnHexCode[3];
    short int OBCId;
    short int StreamId;
    short int LatestFrameId;
    unsigned char FrameUpdateTime[10]; // YY YY 0D DD HH MM SS MSC
    short int State; // 1 -> Active , 0 -> Over
} TmOpDataBufDef;


#endif
