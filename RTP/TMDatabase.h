#ifndef TMDATABASE_H
#define TMDATABASE_H

#include "../includes/SystemFiles.h"
#include "../includes/GlobalMacros.h"
#include "../includes/TMDBReader.h"
#include "../includes/TMShmStruct.h"
#include "../includes/SharedMemoryTemplate.h"
#include "../includes/Constants.h"
#include "../includes/Headers.h"
#include "ParameterInfo.h"

enum TMFormatId
{
    NormalTM = 1,
    CTM = 2,
    BTM = 3,
    F2PB = 4,
    F3PB = 5,
    SPS = 6,
    InvalidTM = 0
};

class TMDatabase
{
public:
    TMDatabase();

    map<int, vector<ParameterInfo> > framePidsMap;
    map<string, uint16_t> pidIndexMap;

    bool InitDatabase(string scId, string tmFormatId);
    bool LoadParameter(string pid);
    void LoadAllParameterDetails();

private:
    TMFormatId tmFormatId;
    TMDBReader *tmDBReader;

    TMFormatId getTMFormatId(string tmFormatId);
};

#endif // TMDATABASE_H
