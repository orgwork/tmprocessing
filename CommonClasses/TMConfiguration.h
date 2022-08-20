#ifndef TMCONFIGURATION_H
#define TMCONFIGURATION_H


#include "../includes/SystemFiles.h"
#include "../includes/GlobalMacros.h"
#include "../includes/StringUtils.h"

class TMConfiguration
{
public:
    TMConfiguration(string scid);

    bool readAllConfigFiles(string &errMsg);
    bool readGlbTmMsgQListConfig(string &errMsg);
    bool readTmOpShmSegConfig(string &errMsg);
    bool isMultiplexed(string stnId, string tmId);

    bool isValidStationId(string stnId);
    bool isValidTmId(string stnId, string tmId);
    bool isValidObcId(string stnId, string tmId, string obcId);

    int getMessageQueueKey(string groupId, bool *ok = nullptr);
    int getShmKey(string stnId, string tmId, string obcId, bool *ok = nullptr);
    int getShmKey(string stnId, string tmId, bool *ok = nullptr);

    vector<string> getStationIds();
    vector<string> getStationIds(string groupId, bool *ok = nullptr);
    vector<string> getTmIds(string stnId, bool *ok = nullptr);
    vector<string> getObcIds(string stnId, string tmId, bool *ok = nullptr);

private:
    string scid;

    map<string, vector<string> > grpStationsMap;                        // grpStationsMap[grp_id]                   -> vector of station Ids
    map<string, int> grpMqKeyMap;                                       // grpMqKeyMap[grp_id]                      -> MsgQueue Key
    map<string, map<string, map<string, int> > > StnTmObcKeyMap;        // StnTmObcKeyMap[Stn_id][Tm_id][Obc_id]    -> shmKey
    map<string, map<string,  int> > StnTmKeyMap;                        // StnTmKeyMap[Stn_id][Tm_id]               -> shmKey


    bool doesStnContainsMultiplexedData(string stnId);
    bool doesStnContainsNonMultiplexedData(string stnId);
};

#endif // TMCONFIGURATION_H
