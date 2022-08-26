#ifndef _ACQ_TO_PROC_DATA_DEF_H
#define _ACQ_TO_PROC_DATA_DEF_H

#include "GlobalMacros.h"

struct AcqToProcMqBufDef
{
    long int message_type ;
    unsigned char OneTmFrame[DATALENGTH];
};


struct TMPacketStructure
{
    unsigned char StnCode;
    unsigned char SatCode[2];
    unsigned char DataType;
    unsigned char TMSource;
    unsigned char ReceivedTime[10];
    unsigned char DerivedTime[10];
    unsigned char DataQuality;
    unsigned char BitRate;
    unsigned char CodingStatus;
    unsigned char SecureNode;
    unsigned char CortexHeader[64];
    unsigned char CortexData[TM_RESP_BUF_SIZE];
    unsigned char CortexTail[4];
};

#endif
