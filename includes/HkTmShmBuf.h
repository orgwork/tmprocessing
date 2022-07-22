#ifndef _HKTMRAWDATABUF_H
#define _HKTMRAWDATABUF_H

#include "GlobalMacros.h"

struct OneProcPidDef
{
    char CDBPid[MAXPIDLEN] ;
    char StringValue[MAXCHARVALFORPID] ;
    double RealValue ;
    long long int TmCount ;
    short int UpdateFrameId ;
    short int PidUpdateTime[7] ; // YYYY MM DD HH MM SS MSC
};

struct HkTmDataBufDef
{
    unsigned char RawHkTmMasterFrame[MAXFRAMECOUNT][MAXWORDCOUNT] ;
    char StnHexCode ;
    short int OBCId ;
    short int StreamId ;
    short int LatestFrameId ;
    //    long int TmAcqCount ;
    //    long int TmRecvCount ;
    OneProcPidDef PrcOpBuf[IMXPID];
};


#endif
