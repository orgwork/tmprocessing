#include "rtpapi.h"
#include "../CommonClasses/Configuration.h"
#include "../includes/HkTmShmBuf.h"


struct rtpApiStruct
{
    string stnId;
    string tmId;
    string obcId;
    string identifier;
    Configuration config;

    map<string, HkTmDataBufDef *> shmMap;
    map<string, int> pidMap;

    rtpApiStruct(string scId): config(scId) {}
};


bool attachToSharedMemory(rtpApiStruct *obj, string identifier, int shmKey, string &errMsg);


rtpApi initRtpApi(char *scId, bool *ok, char *errMsg)
{
    rtpApiStruct *obj = new rtpApiStruct(scId);

    string errorMsg;
    if (!obj->config.readAllConfigFiles(errorMsg))
    {
        *ok = false;
        strncpy(errMsg, errorMsg.c_str(), 500);
        return (void *)obj;
    }

    vector<string> stationsList = obj->config.getStationIds();

    for (size_t i = 0; i < stationsList.size(); i++)
    {
        string stnId = stationsList.at(i);

        vector<string> tmIdList = obj->config.getTmIds(stnId);

        for (size_t j = 0; j < tmIdList.size(); j++)
        {
            string tmId = tmIdList.at(j);

            if (obj->config.isMultiplexed(stnId, tmId))
            {
                vector<string> obcIdList = obj->config.getObcIds(stnId, tmId);

                for (size_t k = 0; k < obcIdList.size(); k++)
                {
                    string obcId = obcIdList.at(k);

                    int shmKey = obj->config.getShmKey(stnId, tmId, obcId);

                    string identifier = stnId + ":" + tmId + ":" + obcId;

                    attachToSharedMemory(obj, identifier, shmKey, errorMsg);
                }
            }
            else
            {
                int shmKey = obj->config.getShmKey(stnId, tmId);

                string identifier = stnId + ":" + tmId;

                attachToSharedMemory(obj, identifier, shmKey, errorMsg);
            }
        }
    }

    *ok = true;
    strncpy(errMsg, errorMsg.c_str(), 500);

    return (void *)obj;
}


bool attachToSharedMemory(rtpApiStruct *obj, string identifier, int shmKey, string &errMsg)
{
    int shmid = shmget(shmKey, 0, 0);
    if (shmid != -1)
    {
        obj->shmMap[identifier] = (HkTmDataBufDef *)shmat(shmid, 0, SHM_RDONLY);

        if (obj->shmMap[identifier]  == (void *) -1)
        {
            errMsg = errMsg + "Failed to attach SHM for " + identifier + "\n";
            return FAILURE;
        }
    }
    else
    {
        errMsg = errMsg + "Failed to attach SHM for " + identifier + "\n";
        return FAILURE;
    }

    return SUCCESS;
}


bool deleteRtpApi(rtpApi obj)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    delete apiStruct;

    return SUCCESS;
}


bool setStationId(rtpApi obj, const char *stnId)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    apiStruct->stnId = stnId;

    if (apiStruct->obcId == "")
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId;
    else
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId + ":" + apiStruct->obcId;


    return SUCCESS;
}


bool setTmId(rtpApi obj, const char *tmId)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    apiStruct->tmId = tmId;

    if (apiStruct->obcId == "")
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId;
    else
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId + ":" + apiStruct->obcId;

    return SUCCESS;
}


bool setObcId(rtpApi obj, const char *obcId)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    apiStruct->obcId = obcId;

    if (apiStruct->obcId == "")
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId;
    else
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId + ":" + apiStruct->obcId;

    return SUCCESS;
}


bool getPidValue(rtpApi obj, const char *pid, double *realValue, char *stringValue, long long int *tmCount)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

//    HkTmDataBufDef *ptr = apiStruct->shmMap[apiStruct->identifier];

//    *realValue = ptr->PrcOpBuf[apiStruct->pidMap[pid]].RealValue;
//    strncpy(stringValue, ptr->PrcOpBuf[apiStruct->pidMap[pid]].StringValue, 14);
//    *tmCount = ptr->PrcOpBuf[apiStruct->pidMap[pid]].TmCount;

    *realValue = 5.0;
    strncpy(stringValue, "ok", 15);
    *tmCount = 105;

    return SUCCESS;
}


bool getRealPidValue(rtpApi obj, const char *pid, double *realValue)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    HkTmDataBufDef *ptr = apiStruct->shmMap[apiStruct->identifier];

    *realValue = ptr->PrcOpBuf[apiStruct->pidMap[pid]].RealValue;

    return SUCCESS;
}


bool getStringPidValue(rtpApi obj, const char *pid, char *stringValue)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    HkTmDataBufDef *ptr = apiStruct->shmMap[apiStruct->identifier];
    stringValue = ptr->PrcOpBuf[apiStruct->pidMap[pid]].StringValue;

    return SUCCESS;
}


bool getRawPidValue(rtpApi obj, const char *pid, long long int *tmCount)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    HkTmDataBufDef *ptr = apiStruct->shmMap[apiStruct->identifier];
    *tmCount = ptr->PrcOpBuf[apiStruct->pidMap[pid]].TmCount;

    return SUCCESS;
}
