#ifndef SHAREDMEMORYCONNECTOR_H
#define SHAREDMEMORYCONNECTOR_H

#include "Configuration.h"


class SharedMemoryConnector
{
public:
    SharedMemoryConnector(string scid);

    bool ConnectToAllSharedMemories(string &errMsg);


private:
    Configuration config;


};

#endif // SHAREDMEMORYCONNECTOR_H
