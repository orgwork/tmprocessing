#include "TMConfiguration.h"


TMConfiguration::TMConfiguration(string scid)
{
    this->scid = scid;
}


// Reads all Config files and stores in maps
bool TMConfiguration::readAllConfigFiles(string &errMsg)
{

    // Reads GlbTmMsgQList Config file
    if (!readGlbTmMsgQListConfig(errMsg))
        return FAILURE;

    // Reads TMOutputShmSegments Config file
    if (!readTmOpShmSegConfig(errMsg))
        return FAILURE;

    return SUCCESS;
}


// Reads GlbTmMsgQList Config file and stores in a map
bool TMConfiguration::readGlbTmMsgQListConfig(string &errMsg)
{
    try
    {
        //  Get the base path of config file from environment variable 'UMACS_PATH'
        char *umacsPath = getenv("UMACS_PATH");

        if (umacsPath == NULL)
        {
            errMsg = "No Environment Variable 'UMACS_PATH' found";
            return FAILURE;
        }

        string basePath = umacsPath;

        string filepath = basePath + "/" + scid + "/TM_PROC/GblTlmMqList_" + scid + ".conf";

        ifstream file;
        //  Open file in read mode
        file.open(filepath);

        //  Return if file could not be opened
        if (!file.is_open())
        {
            errMsg = "Could not open the file: " + filepath;
            return FAILURE;
        }

        string line;

        //  Read from file line by line
        while (getline(file, line))
        {
            //  Trim the line
            line = StringUtils::trim(line);

            //  Split the line with whitespace
            vector<string> keyValues = StringUtils::splitFields(line);

            //  Check if the line is a valid entry
            if (keyValues.size() == 3)
            {
                string groupID = keyValues[0];
                int msgQKey = StringUtils::toInt(keyValues[1]);
                string stations = keyValues[2];

                vector<string> stationList = StringUtils::split(stations, ",");

                this->grpMqKeyMap[groupID] = msgQKey;
                this->grpStationsMap[groupID] = stationList;

            }
            else
                cout << "Invalid Line: " << line << endl;
        }

        //  Close the file
        file.close();
    }
    catch (std::exception &e)
    {
        string exceptionMsg = e.what();
        errMsg = "Failed with Exception in readGlbTmMsgQListConfig: " + exceptionMsg;
        return FAILURE;
    }


    return SUCCESS;
}


// Reads TMOutputShmSegments Config file and stores in a map
bool TMConfiguration::readTmOpShmSegConfig(string &errMsg)
{
    try
    {
        //  Get the base path of config file from environment variable 'UMACS_PATH'
        char *umacsPath = getenv("UMACS_PATH");

        if (umacsPath == NULL)
        {
            errMsg = "No Environment Variable 'UMACS_PATH' found";
            return FAILURE;
        }

        string basePath = umacsPath;

        string filepath = basePath + "/" + scid + "/TM_PROC/TMOutputShmSegments_" + scid + ".conf";

        ifstream file;
        //  Open file in read mode
        file.open(filepath);

        //  Return if file could not be opened
        if (!file.is_open())
        {
            errMsg = "Could not open the file: " + filepath;
            return FAILURE;
        }

        string line;

        int lineCount = 0;
        int baseShmKey = 0;

        //  Read from file line by line
        while (getline(file, line))
        {
            //  Trim the line
            line = StringUtils::trim(line);

            if (lineCount == 0)
            {
                vector<string> fields = StringUtils::splitFields(line);
                baseShmKey = StringUtils::hexToInt(fields.at(0)) * StringUtils::toInt(fields.at(1));
                lineCount++;
                continue;
            }

            //  Split the identifier further to sub-identifiers
            vector<string> identifiers = StringUtils::split(line, ":");

            //  If the identifiers contain station, tm-id & obc-id
            if (identifiers.size() == 3)
            {
                string station = identifiers[0];
                string tmId = identifiers[1];
                string obcId = identifiers[2];

                this->StnTmObcKeyMap[station][tmId][obcId] = baseShmKey + lineCount - 1;
            }

            //  If the identifiers contain only station & tm-id
            else if (identifiers.size() == 2)
            {
                string station = identifiers[0];
                string tmId = identifiers[1];

                this->StnTmKeyMap[station][tmId] = baseShmKey + lineCount - 1;
            }

            lineCount++;
        }

        //  Close the file
        file.close();
    }
    catch (std::exception &e)
    {
        string exceptionMsg = e.what();
        errMsg = "Failed with Exception in readTmOpShmSegConfig: " + exceptionMsg;
        return FAILURE;
    }


    return SUCCESS;
}


int TMConfiguration::getMessageQueueKey(string groupId, bool *ok)
{
    bool isFound = false;
    if (this->grpMqKeyMap.count(groupId))
        isFound = true;

    ok = &isFound;

    int key = this->grpMqKeyMap[groupId];
    return key;
}


int TMConfiguration::getShmKey(string stnId, string tmId, string obcId, bool *ok)
{
    bool isFound = false;
    if (this->StnTmObcKeyMap.count(stnId))
    {
        if (this->StnTmObcKeyMap[stnId].count(tmId))
        {
            if (this->StnTmObcKeyMap[stnId][tmId].count(obcId))
                isFound = true;
        }
    }

    ok = &isFound;

    int key = this->StnTmObcKeyMap[stnId][tmId][obcId];
    return key;
}


int TMConfiguration::getShmKey(string stnId, string tmId, bool *ok)
{
    bool isFound = false;
    if (this->StnTmKeyMap.count(stnId))
    {
        if (this->StnTmKeyMap[stnId].count(tmId))
            isFound = true;
    }

    ok = &isFound;

    int key = this->StnTmKeyMap[stnId][tmId];
    return key;
}


vector<string> TMConfiguration::getStationIds()
{
    vector<string> stationsList;

    for (auto const &it : StnTmObcKeyMap)
        stationsList.push_back(it.first);

    for (auto const &it : StnTmKeyMap)
        stationsList.push_back(it.first);

    unique(stationsList.begin(), stationsList.end());

    return stationsList;
}


vector<string> TMConfiguration::getStationIds(string groupId, bool *ok)
{
    bool isFound = false;
    if (this->grpStationsMap.count(groupId))
        isFound = true;

    ok = &isFound;

    vector<string> stationsList = this->grpStationsMap[groupId];
    return stationsList;
}


vector<string> TMConfiguration::getTmIds(string stnId, bool *ok)
{
    bool isFound = false;
    if (this->StnTmObcKeyMap.count(stnId))
        isFound = true;

    ok = &isFound;


    vector<string> tmIdList;

    if (doesStnContainsMultiplexedData(stnId))
    {
        map<string, map<string, int>> tmIdsList = this->StnTmObcKeyMap[stnId];

        for (auto const &it : tmIdsList)
            tmIdList.push_back(it.first);
    }

    if (doesStnContainsNonMultiplexedData(stnId))
    {
        map<string, int> tmIdsList = this->StnTmKeyMap[stnId];

        for (auto const &it : tmIdsList)
            tmIdList.push_back(it.first);
    }


    return tmIdList;
}


bool TMConfiguration::doesStnContainsMultiplexedData(string stnId)
{
    if (this->StnTmObcKeyMap.count(stnId))
        return true;
    else
        return false;
}


bool TMConfiguration::doesStnContainsNonMultiplexedData(string stnId)
{
    if (this->StnTmKeyMap.count(stnId))
        return true;
    else
        return false;
}


bool TMConfiguration::isMultiplexed(string stnId, string tmId)
{
    if (this->StnTmObcKeyMap[stnId].count(tmId))
        return true;
    else
        return false;
}


vector<string> TMConfiguration::getObcIds(string stnId, string tmId, bool *ok)
{
    bool isFound = false;
    if (this->StnTmObcKeyMap[stnId].count(tmId))
        isFound = true;

    ok = &isFound;

    map<string, int> obcIds = this->StnTmObcKeyMap[stnId][tmId];

    vector<string> obcIdList;

    for (auto const &it : obcIds)
        obcIdList.push_back(it.first);

    return obcIdList;
}


bool TMConfiguration::isValidStationId(string stnId)
{
    if (this->StnTmObcKeyMap.count(stnId))
        return true;

    if (this->StnTmKeyMap.count(stnId))
        return true;

    return false;
}


bool TMConfiguration::isValidTmId(string stnId, string tmId)
{
    if (this->StnTmObcKeyMap[stnId].count(tmId))
        return true;

    if (this->StnTmKeyMap[stnId].count(tmId))
        return true;

    return false;
}


bool TMConfiguration::isValidObcId(string stnId, string tmId, string obcId)
{
    if (this->StnTmObcKeyMap[stnId][tmId].count(obcId))
        return true;

    return false;
}
