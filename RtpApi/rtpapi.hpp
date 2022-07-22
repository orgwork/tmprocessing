#ifndef RTPAPI_H
#define RTPAPI_H

#include "rtpapi.h"
#include "../includes/SystemFiles.h"

class RtpApi
{
public:
    RtpApi(string scId);
    ~RtpApi();

    bool Init(string &errMsg);
    bool SetStationId(string stnId, string &errMsg);
    bool SetTmId(string tmId, string &errMsg);
    bool SetObcId(string obcId, string &errMsg);

    double GetProcessedRealValue(string pid);
    string GetProcessedStringValue(string pid);
    uint64_t GetRawTmCount(string pid);

private:
    string stnId;
    string tmId;
    string obcId;
};

#endif // RTPAPI_H
