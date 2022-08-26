#ifndef PROCESSPARAMETERS_H
#define PROCESSPARAMETERS_H

#include "TMDatabase.h"
#include "MasterFrame.h"
#include "TMDataType.h"
#include "../includes/HkTmShmBuf.h"

class ProcessParameters
{
public:
    ProcessParameters();

    void ProcessFrame(char *rawFrame);
    void ProcessParameter(ParameterInfo *parameter);
    void UpdateRawValue(ParameterInfo *parameter);
    void UpdateProcessedValue(ParameterInfo *parameter);

    TMDatabase *tmdb;
    MasterFrame *masterFrame;
    TMDataType tmDataType;
    TMProcessingType tmProcType;
    TmOpDataBufDef *ptrTmOpDataBufDef;

private:
    int curFrameId;

};

#endif // PROCESSPARAMETERS_H