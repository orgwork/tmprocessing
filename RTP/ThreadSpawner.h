#ifndef THREADSPAWNER_H
#define THREADSPAWNER_H

#include <../includes/SystemFiles.h>
#include <../includes/GlobalMacros.h>
#include <../includes/MqBufDefs.h>
#include <../includes/HkTmShmBuf.h>
#include <../CommonClasses/TMConfiguration.h>
#include "TMDatabase.h"

struct threadData
{
    key_t shmKey;
    string threadId;
    sem_t dataEmpty;
    sem_t dataFilled;
    queue<AcqToProcMqBufDef> packets;
    TmOpDataBufDef *ptrHkTmDataBuf;
    TMDatabase *db;
    exception_ptr teptr;
};

class ThreadSpawner
{
public:
    ThreadSpawner(string scId, string MsgQIdentifier);

    bool Init(string &errMsg);
    bool AttachToShmSegments(string &errMsg);
    bool SpawnThreads(string &errMsg);
    bool StartProcessing(string &errMsg);

private:
    TMConfiguration config;
    string scId;
    string MsgQIdentifier;
    bool isAppRunning;
    map<string, threadData *> threadMap;
    mutex printMutex;
    int64_t counter;
    TMDatabase hkTMDB;

    void WorkerThread(threadData *threadInfo);
    void SendNewFrameToWorkerThread(string identifier, AcqToProcMqBufDef message);

};

#endif // THREADSPAWNER_H
