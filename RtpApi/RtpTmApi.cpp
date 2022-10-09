#include "RtpTmApi.hpp"
#include "../CommonClasses/TMConfiguration.h"
#include "../includes/HkTmShmBuf.h"
#include "../includes/TMDBReader.h"

RtpTmApi::RtpTmApi()
{

}


bool RtpTmApi::InitRtpTmApi(string scId, string &errorMsg)
{
    this->config = new TMConfiguration(scId);
    this->tmDBReader = new TMDBReader;

    if (!config->readAllConfigFiles(errorMsg))
        return false;

    vector<string> stationsList = this->config->getStationIds();

    for (size_t i = 0; i < stationsList.size(); i++)
    {
        string stnId = stationsList.at(i);

        vector<string> tmIdList = this->config->getTmIds(stnId);

        for (size_t j = 0; j < tmIdList.size(); j++)
        {
            string tmId = tmIdList.at(j);

            if (this->config->isMultiplexed(stnId, tmId))
            {
                vector<string> obcIdList = this->config->getObcIds(stnId, tmId);

                for (size_t k = 0; k < obcIdList.size(); k++)
                {
                    string obcId = obcIdList.at(k);

                    int shmKey = this->config->getShmKey(stnId, tmId, obcId);

                    string identifier = stnId + ":" + tmId + ":" + obcId;

                    attachToSharedMemory(identifier, shmKey, errorMsg);
                }
            }
            else
            {
                int shmKey = this->config->getShmKey(stnId, tmId);

                string identifier = stnId + ":" + tmId;

                attachToSharedMemory(identifier, shmKey, errorMsg);
            }
        }
    }

    if (this->shmMap.size() < 1)
        return false;


    try
    {
        string scId = "NIS-01";
        if (this->tmDBReader->init((char *)scId.c_str(), 0) == false)
        {
            cout << "failed to load database" << endl;
            exit(EXIT_FAILURE);
        }

        bool     retSts = false;
        uint8_t *errMsg = nullptr;

        // TODO: UPDATE FROM SC CONFIG
        int tmFormatId = 1;

        int            numOfPids = this->tmDBReader->glbTMDB_ptr->getTotalNumPids(tmFormatId, &retSts, errMsg);
        char         **pids      = (char **)this->tmDBReader->glbTMDB_ptr->getPIDList(tmFormatId, &retSts, errMsg);
        vector<string> pidVector(pids, pids + numOfPids);

        for (size_t i = 0; i < pidVector.size(); i++)
        {
            string cdbPid   = pidVector.at(i);

            uint8_t *pid = (uint8_t *)cdbPid.c_str();
            int pidIndex = this->tmDBReader->glbTMDB_ptr->getPIDIndex(pid, &retSts, errMsg);

            this->pidMap[cdbPid] = pidIndex;
        }
    }
    catch (std::exception &e)
    {
        cout << e.what() << endl;
    }

    // temporary code
    if (this->pidMap.size() < 1)
        return false;

    return true;
}


void RtpTmApi::SetStationId(string stnId)
{
    this->stnId = stnId;

    if (this->obcId == "")
        this->identifier = this->stnId + ":" + this->tmId;
    else
        this->identifier = this->stnId + ":" + this->tmId + ":" + this->obcId;

}


void RtpTmApi::SetTmId(string tmId)
{
    this->tmId = tmId;

    if (this->obcId == "")
        this->identifier = this->stnId + ":" + this->tmId;
    else
        this->identifier = this->stnId + ":" + this->tmId + ":" + this->obcId;

}

void RtpTmApi::SetObcId(string obcId)
{
    this->obcId = obcId;

    if (this->obcId == "")
        this->identifier = this->stnId + ":" + this->tmId;
    else
        this->identifier = this->stnId + ":" + this->tmId + ":" + this->obcId;

}


double RtpTmApi::GetProcessedRealValue(string pid)
{

}


string RtpTmApi::GetProcessedStringValue(string pid)
{

}


uint64_t RtpTmApi::GetRawTmCount(string pid)
{

}


bool RtpTmApi::attachToSharedMemory(string identifier, int shmKey, string &errMsg)
{
    int shmid = shmget(shmKey, 0, 0);
    if (shmid != -1)
    {
        shmMap[identifier] = (TmOpDataBufDef *)shmat(shmid, 0, SHM_RDONLY);

        if (shmMap[identifier]  == (void *) -1)
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
