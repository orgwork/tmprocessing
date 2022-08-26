#ifndef TMPROCESSOR_HPP
#define TMPROCESSOR_HPP

#include "../includes/SystemFiles.h"
#include "../includes/GlobalMacros.h"
#include <../includes/HkTmShmBuf.h>
#include "TMDatabase.h"
#include "MasterFrame.h"
#include "ProcessParameters.h"

class TMProcessor
{
public:
    TMProcessor(string satId, string tmFormat);

    void SetTMDatabase(TMDatabase *tmdb);
    bool InitTMProcessor(string &errMsg);
    bool AddParameter(string pid, string &errMsg);
    void AddAllParameters();
    bool RemoveParameter(string pid, string &errMsg);
    void RemoveAllParameters();
    void ProcessFrame(char *frame);
    double GetProcessedRealValue(string pid);
    string GetProcessedStringValue(string pid);
    uint64_t GetTMRawCount(string pid);
    void SetSharedMemoryPointer(TmOpDataBufDef *ptr);

private:
    string satId;
    string tmFormat;

    TMDatabase *tmdb;
    ProcessParameters *processParams;
    TmOpDataBufDef *ptrTmOpDataBufDef;
};

#endif // TMPROCESSOR_HPP
