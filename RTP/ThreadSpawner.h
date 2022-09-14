#ifndef THREADSPAWNER_H
#define THREADSPAWNER_H

#include <../includes/SystemFiles.h>
#include <../includes/GlobalMacros.h>
#include <../includes/MqBufDefs.h>
#include <../includes/HkTmShmBuf.h>
#include <../CommonClasses/TMConfiguration.h>
#include "TMDatabase.h"
#include "TMProcessor.hpp"

struct threadData
{
    key_t shmKey;
    string threadId;
    sem_t dataEmpty;
    sem_t dataFilled;
    queue<TMPacketStructure> packets;
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
    TMDatabase *hkTMDB;
    vector<thread *> processingThreads;

    void WorkerThread(threadData *threadInfo);
    void SendNewFrameToWorkerThread(string identifier, TMPacketStructure message);
    void ThrowMsgGetExceptions();
    void PrintMsgRcvErrors(int msgQId);

};

#endif // THREADSPAWNER_H
