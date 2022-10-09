#ifndef RTPTMAPI_H
#define RTPTMAPI_H

#include <string>
#include <map>

using namespace std;

struct TmOpDataBufDef;

class TMConfiguration;

class TMDBReader;

class RtpTmApi
{
public:
    RtpTmApi();

    bool InitRtpTmApi(string scId, string &errorMsg);
    void SetStationId(string stnId);
    void SetTmId(string tmId);
    void SetObcId(string obcId);
    bool validate(string &errorMsg);

    double GetProcessedRealValue(string pid);
    string GetProcessedStringValue(string pid);
    uint64_t GetRawTmCount(string pid);

private:
    bool attachToSharedMemory(string identifier, int shmKey, string &errMsg);

    string stnId;
    string tmId;
    string obcId;
    string identifier;
    TMConfiguration *config;
    TMDBReader *tmDBReader;
    bool isValidated;

    map<string, TmOpDataBufDef *> shmMap;
    map<string, int> pidMap;
};

#endif // RTPTMAPI_H
