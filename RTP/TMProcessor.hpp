#ifndef TMPROCESSOR_HPP
#define TMPROCESSOR_HPP

#include <string>

using namespace std;

class TMProcessor
{
public:
    TMProcessor();
    ~TMProcessor();

    bool InitTMProcessor(string scId, string tmFormat, string &errMsg); //2

    bool AddParameter(string pid, string &errMsg); //3

    void AddAllParameters();

    bool RemoveParameter(string pid, string &errMsg);
    void RemoveAllParameters();

    void ProcessFrame(char *frame);

    double GetProcessedRealValue(string pid);
    string GetProcessedStringValue(string pid);
    uint64_t GetTMRawCount(string pid);

    // Only for RT Processing
    void SetTMDatabase(void *tmdbPtr);
    void SetSharedMemoryPointer(void *TmOpDataBufDefPtr);

private:
    string scId;
    string tmFormat;

    void *tmdbPtr;
    void *processParamsPtr;
    void *TmOpDataBufDefPtr;
};

#endif // TMPROCESSOR_HPP
