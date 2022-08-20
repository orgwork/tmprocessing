#ifndef _ACQ_TO_PROC_DATA_DEF_H
#define _ACQ_TO_PROC_DATA_DEF_H

#include "GlobalMacros.h"

struct AcqToProcMqBufDef
{
    long int message_type ;
    unsigned char OneTmFrame[DATALENGTH];
};


struct TMPacketStructure{
    unsigned char StnCode[2];
    unsigned char SatCode[2];
    unsigned char DataType[2];
    unsigned char TMSource[2];
    unsigned char ReceivedTime[8];
    unsigned char DerivedTime[8];
    unsigned char DataQuality[2];
    unsigned char BitRate[2];
    unsigned char CodingStatus[2];
    unsigned char SecureNode[2];
    unsigned char CortexData[TM_RESP_BUF_SIZE];
};

#endif
