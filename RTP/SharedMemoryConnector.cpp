#include "SharedMemoryConnector.h"

SharedMemoryConnector::SharedMemoryConnector(string scid): config(scid)
{

}


bool SharedMemoryConnector::ConnectToAllSharedMemories(string &errMsg)
{
    if (!this->config.readAllConfigFiles(errMsg))
        return FAILURE;

    vector<string> stationsList = this->config.getStationIds();

    for (size_t i = 0; i < stationsList.size(); i++)
    {
        string stnId = stationsList.at(i);

        vector<string> tmIdList = this->config.getTmIds(stnId);

        for (size_t j = 0; j < tmIdList.size(); j++)
        {
            string tmId = tmIdList.at(j);

            if (this->config.isMultiplexed(stnId, tmId))
            {
                vector<string> obcIdList = this->config.getObcIds(stnId, tmId);

                for (size_t k = 0; k < obcIdList.size(); k++)
                {
                    string obcId = obcIdList.at(k);

                    int shmKey = this->config.getShmKey(stnId, tmId, obcId);

                }
            }
        }

    }

    return SUCCESS;
}
