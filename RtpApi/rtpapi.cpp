#include "rtpapi.h"
#include "../CommonClasses/TMConfiguration.h"
#include "../includes/HkTmShmBuf.h"


struct rtpApiStruct
{
    string stnId;
    string tmId;
    string obcId;
    string identifier;
    TMConfiguration config;
    bool isValidated;

    map<string, TmOpDataBufDef *> shmMap;
    map<string, int> pidMap;

    rtpApiStruct(string scId): config(scId) {}
};


bool attachToSharedMemory(rtpApiStruct *obj, string identifier, int shmKey, string &errMsg)
{
    int shmid = shmget(shmKey, 0, 0);
    if (shmid != -1)
    {
        obj->shmMap[identifier] = (TmOpDataBufDef *)shmat(shmid, 0, SHM_RDONLY);

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

    if (obj->shmMap.size() > 0)
        *ok = true;
    else
        *ok = false;

// temporary code
    char *umacsPath = getenv("UMACS_PATH");
    if (umacsPath == NULL)
    {
        errorMsg = "No Environment Variable 'UMACS_PATH' found";
        *ok = false;
    }
    string basePath = umacsPath;
    string filepath = basePath + "/" + scId + "/TM_PROC/CDBId_PidNum." + scId ;
    ifstream file;
    file.open(filepath);
    if (!file.is_open())
    {
        errorMsg = "Could not open the file: " + filepath;
        *ok = false;
    }
    else
    {
        string line;
        int index = 0;
        while (getline(file, line))
        {
            line = StringUtils::trim(line);
            vector<string> fields = StringUtils::splitFields(line);
            obj->pidMap[fields.at(0)] = index;
            index++;
        }
    }
// temporary code
    if (obj->pidMap.size() > 0)
        *ok = true;
    else
        *ok = false;

    strncpy(errMsg, errorMsg.c_str(), 500);

    return (void *)obj;
}


bool deleteRtpApi(rtpApi obj)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    delete apiStruct;

    return SUCCESS;
}


bool setStationId(rtpApi obj, const char *stnId, char *errMsg)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    apiStruct->stnId = stnId;

    if (apiStruct->obcId == "")
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId;
    else
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId + ":" + apiStruct->obcId;


    return SUCCESS;
}


bool setTmId(rtpApi obj, const char *tmId, char *errMsg)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    apiStruct->tmId = tmId;

    if (apiStruct->obcId == "")
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId;
    else
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId + ":" + apiStruct->obcId;

    return SUCCESS;
}


bool setObcId(rtpApi obj, const char *obcId, char *errMsg)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    apiStruct->obcId = obcId;

    if (apiStruct->obcId == "")
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId;
    else
        apiStruct->identifier = apiStruct->stnId + ":" + apiStruct->tmId + ":" + apiStruct->obcId;

    return SUCCESS;
}


bool validate(rtpApi obj, char *errMsg)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    if (!apiStruct->config.isValidStationId(apiStruct->stnId))
    {
        strncpy(errMsg, "INVALID STN-ID", 500);
        return FAILURE;
    }

    if (!apiStruct->config.isValidTmId(apiStruct->stnId, apiStruct->tmId))
    {
        strncpy(errMsg, "INVALID TM-ID", 500);
        return FAILURE;
    }


    if (apiStruct->config.isMultiplexed(apiStruct->stnId, apiStruct->tmId))
    {
        if (!apiStruct->config.isValidObcId(apiStruct->stnId, apiStruct->tmId, apiStruct->obcId))
        {
            strncpy(errMsg, "INVALID OBC-ID", 500);
            return FAILURE;
        }
    }


    apiStruct->isValidated = true;

    return SUCCESS;
}


bool getPidVal(rtpApi obj, const char *pid, double *realValue, char *stringValue, long long int *tmCount)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    if (!apiStruct->isValidated)
        return FAILURE;

    int pidIndex = apiStruct->pidMap[pid];

    TmOpDataBufDef *ptr = apiStruct->shmMap[apiStruct->identifier];

    *realValue = ptr->HkTmPrcOpBuf[pidIndex].RealValue;
    strncpy(stringValue, ptr->HkTmPrcOpBuf[pidIndex].StringValue, 15);
    *tmCount = ptr->HkTmPrcOpBuf[pidIndex].TmCount;

    return SUCCESS;
}


bool getRealPidValue(rtpApi obj, const char *pid, double *realValue)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    if (!apiStruct->isValidated)
        return FAILURE;

    TmOpDataBufDef *ptr = apiStruct->shmMap[apiStruct->identifier];

    *realValue = ptr->HkTmPrcOpBuf[apiStruct->pidMap[pid]].RealValue;

    return SUCCESS;
}


bool getStringPidValue(rtpApi obj, const char *pid, char *stringValue)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    if (!apiStruct->isValidated)
        return FAILURE;

    TmOpDataBufDef *ptr = apiStruct->shmMap[apiStruct->identifier];
    strncpy(stringValue, ptr->HkTmPrcOpBuf[apiStruct->pidMap[pid]].StringValue, 15);

    return SUCCESS;
}


bool getRawPidValue(rtpApi obj, const char *pid, long long int *tmCount)
{
    rtpApiStruct *apiStruct = (rtpApiStruct *)obj;

    if (!apiStruct->isValidated)
        return FAILURE;

    TmOpDataBufDef *ptr = apiStruct->shmMap[apiStruct->identifier];
    *tmCount = ptr->HkTmPrcOpBuf[apiStruct->pidMap[pid]].TmCount;

    return SUCCESS;
}
