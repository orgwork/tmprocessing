#ifndef RTPAPI_H
#define RTPAPI_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void *rtpApi;

// Initialization
rtpApi initRtpApi(char *scId, bool *ok, char *errMsg);

// Destructor -- Call after use
bool deleteRtpApi(rtpApi obj);

// Set Station Id -- eg: HSN, BLR
bool setStationId(rtpApi obj, const char *stnId, char *errMsg);

// Set TM Id -- eg: TM1, TM2
bool setTmId(rtpApi obj, const char *tmId, char *errMsg);

// Set OBC Id -- eg: MC1, MC2, MC3
bool setObcId(rtpApi obj, const char *obcId, char *errMsg);

// Validate the details provided
bool validate(rtpApi obj, char *errMsg);

// Get PID Values -- Processed real value, Processed string value, TM Raw Count
bool getPidVal(rtpApi obj, const char *pid, double *realValue, char *stringValue, long long int *tmCount);

bool getPidValues( rtpApi obj, short noOfPids, char pidMnm[], double enggVal[], char strVal[][], long long int rawVal[], int   *frmTimeInMS, int  *dataBrkFlg, char errMsg[] );

// Get PID Values -- Processed real value
bool getRealPidValue(rtpApi obj, const char *pid, double *realValue);

// Get PID Values -- Processed string value
bool getStringPidValue(rtpApi obj, const char *pid, char *stringValue);

// Get PID Values -- TM Raw Count
bool getRawPidValue(rtpApi obj, const char *pid, long long int *tmCount);

#ifdef __cplusplus
}
#endif

#endif // RTPAPI_H
