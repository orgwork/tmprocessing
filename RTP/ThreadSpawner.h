#ifndef THREADSPAWNER_H
#define THREADSPAWNER_H

#include <../includes/SystemFiles.h>
#include <../includes/GlobalMacros.h>
#include <../includes/MqBufDefs.h>
#include <../includes/HkTmShmBuf.h>
#include <../CommonClasses/Configuration.h>

struct threadData
{
    key_t shmKey;
    string threadId;
    sem_t dataEmpty;
    sem_t dataFilled;
    queue<AcqToProcMqBufDef> packets;
    HkTmDataBufDef *ptrHkTmDataBuf;
    exception_ptr teptr;
};

class ThreadSpawner
{
public:
    ThreadSpawner(string scid, string MsgQIdentifier);

    bool Init(string &errMsg);
    bool AttachToShmSegments(string &errMsg);
    bool SpawnThreads(string &errMsg);
    bool StartProcessing(string &errMsg);

private:
    Configuration config;
    string scid;
    string MsgQIdentifier;
    bool isAppRunning;
    map<string, threadData *> threadMap;
    mutex printMutex;
    int64_t counter;


    void WorkerThread(threadData *threadInfo);
    void SendNewFrameToWorkerThread(string identifier, AcqToProcMqBufDef message);

};

#endif // THREADSPAWNER_H
