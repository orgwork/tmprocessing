#ifndef TMDATABASE_H
#define TMDATABASE_H

#include "../includes/GlobalMacros.h"
#include "../includes/SystemFiles.h"
#include "../includes/TMDBReader.h"
#include "ParameterInfo.h"

enum TMFormatId
{
    NormalTM  = 1,
    CTM       = 2,
    BTM       = 3,
    F2PB      = 4,
    F3PB      = 5,
    SPS       = 6,
    InvalidTM = 0
};

class TMDatabase
{
public:
    TMDatabase();

    map<int, vector<int>> frameIdPidIndexListMap;
    ParameterInfo *pidIndexParameterMap[MAXPID];
    map<string, int> pidIndexMap;

    bool InitDatabase(string scId, string tmFormatId);
    bool LoadParameter(string paramId, string &errorMsg);
    void LoadAllParameterDetails();
    bool RemoveParameter(string paramId, string &errorMsg);
    void RemoveAllParameters();

private:
    TMFormatId  tmFormatId;
    TMDBReader *tmDBReader;

    TMFormatId getTMFormatId(string tmFormatId);
};

#endif // TMDATABASE_H
