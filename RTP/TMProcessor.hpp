#ifndef TMPROCESSOR_HPP
#define TMPROCESSOR_HPP

#include "../includes/SystemFiles.h"
#include "../includes/GlobalMacros.h"

class TMProcessor
{
public:
    TMProcessor(string satId, string tmFormat);

    bool InitTMProcessor(string &errMsg);
    bool AddParameter(string pid, string &errMsg);
    bool RemoveParameter(string pid, string &errMsg);
    bool ProcessFrame(char *frame);
    double GetProcessedRealValue(string pid);
    string GetProcessedStringValue(string pid);
    uint64_t GetTMRawCount(string pid);

private:
    string satId;
    string tmFormat;
};

#endif // TMPROCESSOR_HPP
