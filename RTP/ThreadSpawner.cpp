#include "ThreadSpawner.h"

ThreadSpawner::ThreadSpawner(string scId, string MsgQIdentifier): config(scId)
{
    this->scId = scId;
    this->MsgQIdentifier = MsgQIdentifier;
    this->isAppRunning = true;

    this->hkTMDB = new TMDatabase;
}


//  Initialisation: updates threadMap with keys for each applicable stream
bool ThreadSpawner::Init(string &errMsg)
{
    if (!this->config.readAllConfigFiles(errMsg))
        return FAILURE;

    vector<string> stationsList = this->config.getStationIds(MsgQIdentifier);

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

                    string identifier = stnId + ":" + tmId + ":" + obcId;

                    threadData *td = new threadData;
                    td->shmKey = shmKey;
                    td->threadId = identifier;

                    this->threadMap[identifier] = td;
                }
            }
            else
            {
                int shmKey = this->config.getShmKey(stnId, tmId);

                string identifier = stnId + ":" + tmId;

                threadData *td = new threadData;
                td->shmKey = shmKey;
                td->threadId = identifier;

                this->threadMap[identifier] = td;
            }
        }
    }

    if (hkTMDB->InitDatabase(scId, "HKTM") == false)
    {
        errMsg = "Database Initialisation Failure";
        return FAILURE;
    }

    return SUCCESS;
}


//  Attach to all Shared Memory segments and store pointers in threadMap
bool ThreadSpawner::AttachToShmSegments(string &errMsg)
{
    //    Iterate through threadMap
    for (auto const &it : threadMap)
    {
        string identifier = it.first;
        threadData *td = it.second;

        //        Get the Shared memory segment Id
        int shmid = shmget(td->shmKey, 0, 0);

        if (shmid != -1)
        {
            //            Store Pointer of the shared memory
            this->threadMap[identifier]->ptrHkTmDataBuf = (TmOpDataBufDef *)shmat(shmid, 0, 0);

            if (this->threadMap[identifier]->ptrHkTmDataBuf  == (void *) -1)
            {
                errMsg = "Failed to attach SHM for " + identifier;
                return FAILURE;
            }
        }
        else
        {
            errMsg = "Failed to attach SHM for " + identifier;
            return FAILURE;
        }

    }

    return SUCCESS;
}


//  Spawns a Thread for each Stream
bool ThreadSpawner::SpawnThreads(string &errMsg)
{
    for (auto const &it : threadMap)
    {
        string identifier = it.first;
        threadData *td = it.second;

        sem_init(&td->dataEmpty, 0, 10);
        sem_init(&td->dataFilled, 0, 0);

        // Create a thread and pass the stream thread information to the thread
        thread *newThread = new thread(&ThreadSpawner::WorkerThread, this, td);
    }

    return SUCCESS;
}


//  Worker thread for handling the message
void ThreadSpawner::WorkerThread(threadData *threadInfo)
{
    try
    {
        bool retSts = FAILURE;
        string errMsg = "";

        TMProcessor tmProcessor(scId, "HKTM");

        tmProcessor.SetTMDatabase(hkTMDB);
        tmProcessor.SetSharedMemoryPointer(threadInfo->ptrHkTmDataBuf);

        retSts = tmProcessor.InitTMProcessor(errMsg);
        if (retSts == FAILURE)
        {
            cout << errMsg << endl;
            return;
        }

        while (isAppRunning)
        {
            //  wait until new message packet arrives
            sem_wait(&threadInfo->dataFilled);

            short frameId = threadInfo->packets.front().CortexData[10] & 0x1f;

            memcpy(threadInfo->ptrHkTmDataBuf->RawHkTmMasterFrame[frameId].OneRawHkTmFrame, &threadInfo->packets.front().CortexData, 256);

            tmProcessor.ProcessFrame((char *)threadInfo->packets.front().CortexData);

            threadInfo->ptrHkTmDataBuf->LatestFrameId = frameId;

            //            for (int i = 0; i < 256; i++)
            //                printf(" %02X ", threadInfo->packets.front().CortexData[i]);

            //            cout << endl;

            //            printMutex.lock();
            //            cout << "Thread Id: " << threadInfo->threadId << "  " << frameId << endl;
            //            printMutex.unlock();

            threadInfo->packets.pop();

            sem_post(&threadInfo->dataEmpty);
        }
    }
    catch (std::exception &e)
    {
        threadInfo->teptr = current_exception();
        cout << e.what() << endl;
        sem_post(&threadInfo->dataEmpty);
    }
}


//  Starts receiving messages from message queue and passes onto the threads
bool ThreadSpawner::StartProcessing(string &errMsg)
{
    int msgQKey = this->config.getMessageQueueKey(MsgQIdentifier);

    int msgQId;

    if ((msgQId = msgget(msgQKey, 0644)) == -1)
    {
        perror("msgget");
        return EXIT_FAILURE;
    }

    AcqToProcMqBufDef message;

    while (msgrcv(msgQId, &message, sizeof (message), 0, 0))
    {
        TMPacketStructure tmPkt;

        memcpy(&tmPkt, message.OneTmFrame, DATALENGTH);

        int tmId = (tmPkt.CortexData[10] & 0x20) >> 5;

        string identifier, tmType;
        //        char stnId[5];

        if (tmId == 0)
            tmType = "HKTM1";
        else
            tmType = "HKTM2";

        //        memcpy(stnId, message.OneTmFrame, 4);
        //        stnId[4] = '\0';

        identifier =  "STN1:" + tmType;
        this->SendNewFrameToWorkerThread(identifier, tmPkt);

        identifier = "ALL:" + tmType;
        this->SendNewFrameToWorkerThread(identifier, tmPkt);
    }

    return SUCCESS;
}


void ThreadSpawner::SendNewFrameToWorkerThread(string identifier, TMPacketStructure message)
{
    threadData *td = threadMap[identifier];

    if (td == nullptr)
        return;

    sem_wait(&td->dataEmpty);

    // Handling exceptions from thread
    if (td->teptr)
    {
        try
        {
            std::rethrow_exception(td->teptr);
        }
        catch (const std::exception &ex)
        {
            // To free memory

            cerr << "Thread-" << td->threadId << " exited with exception: " << ex.what() << "\n";

            td->teptr = nullptr;

            thread *newThread = new thread(&ThreadSpawner::WorkerThread, this, td);
        }
    }

    //        Push to Internal queue
    td->packets.push(message);

    sem_post(&td->dataFilled);
}
