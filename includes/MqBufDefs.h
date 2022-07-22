#ifndef _ACQ_TO_PROC_DATA_DEF_H
#define _ACQ_TO_PROC_DATA_DEF_H

#include "GlobalMacros.h"

struct AcqToProcMqBufDef
{
    long int message_type ;
    unsigned char OneTmFrame[DATALENGTH];
} ;

#endif
