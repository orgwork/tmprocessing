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
                    this->prevFrameIdMap[identifier] = -1;
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
                this->prevFrameIdMap[identifier] = -1;
            }
        }
    }

    if (hkTMDB->InitDatabase(scId, "HKTM") == false)
    {
        errMsg = "Database Initialisation Failure";
        return FAILURE;
    }
    else
        hkTMDB->LoadAllParameterDetails();

    return SUCCESS;
}


//  Attach to all Shared Memory segments and store pointers in threadMap
bool ThreadSpawner::AttachToShmSegments(string &errMsg)
{
    try
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
    catch (exception &e)
    {
        errMsg = e.what();
        return FAILURE;
    }
}


//  Spawns a Thread for each Stream
bool ThreadSpawner::SpawnThreads(string &errMsg)
{
    try
    {
        for (auto const &it : threadMap)
        {
            string identifier = it.first;
            threadData *td = it.second;

            sem_init(&td->dataEmpty, 0, 10);
            sem_init(&td->dataFilled, 0, 0);

            // Create a thread and pass the stream thread information to the thread
            thread *newThread = new thread(&ThreadSpawner::WorkerThread, this, td);
            this->processingThreads.push_back(newThread);
        }

        return SUCCESS;
    }
    catch (exception &e)
    {
        errMsg = e.what();
        return FAILURE;
    }
}


//  Worker thread for handling the message
void ThreadSpawner::WorkerThread(threadData *threadInfo)
{
    try
    {
        bool retSts = FAILURE;
        string errMsg = "";

        TMProcessor tmProcessor;

        tmProcessor.SetTMDatabase(hkTMDB);
        tmProcessor.SetSharedMemoryPointer(threadInfo->ptrHkTmDataBuf);

        memcpy(threadInfo->ptrHkTmDataBuf->ScId, scId.c_str(), 6);

        retSts = tmProcessor.InitTMProcessor(scId, "HKTM", errMsg);
        if (retSts == FAILURE)
        {
            cout << errMsg << endl;
            return;
        }

        string tmOpFileName = "/tmp/" + threadInfo->threadId + "_breaks.log";
        ofstream tmop_file(tmOpFileName);

        while (isAppRunning)
        {
            //  wait until new message packet arrives
            sem_wait(&threadInfo->dataFilled);

            auto startTime = chrono::high_resolution_clock::now();

            TMPacketStructure tmPkt = threadInfo->packets.front();

            threadInfo->ptrHkTmDataBuf->State = ACTIVE;

            short frameId = tmPkt.CortexData[10] & 0x1f;

            memcpy(threadInfo->ptrHkTmDataBuf->RawHkTmMasterFrame[frameId].OneRawHkTmFrame, tmPkt.CortexData, 256);

            memcpy(threadInfo->ptrHkTmDataBuf->FrameUpdateTime, tmPkt.ReceivedTime, 10);

            tmProcessor.ProcessFrame((char *)tmPkt.CortexData);

            threadInfo->ptrHkTmDataBuf->LatestFrameId = frameId;

            threadInfo->ptrHkTmDataBuf->State = OVER;

            this->PrintPacketDetails(tmPkt);

            threadInfo->packets.pop();

            // Make the slot free for new packet
            sem_post(&threadInfo->dataEmpty);

            auto endTime = chrono::high_resolution_clock::now();

            printMutex.lock();
            cout << "Processed Frame: " << frameId << " in " << chrono::duration_cast<chrono::microseconds>(endTime - startTime).count() << " us" << endl;
            printMutex.unlock();

            // Data Break Check
            int prevFrameId = this->prevFrameIdMap[threadInfo->threadId];
            int expectedFrameId;

            if (prevFrameId == 31)
                expectedFrameId = 0;
            else if (prevFrameId == -1)
                expectedFrameId = frameId;
            else
                expectedFrameId = prevFrameId + 1;

            if (expectedFrameId != frameId)
            {
                printMutex.lock();
                cout << "Expected Frame Id: " << expectedFrameId << " Received Frame Id: " << frameId << endl;
                printMutex.unlock();

                tmop_file << "Expected Frame Id: " << expectedFrameId << " Received Frame Id: " << frameId << endl;
            }

            this->prevFrameIdMap[threadInfo->threadId] = frameId;
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
    try
    {
        int msgQKey = this->config.getMessageQueueKey(MsgQIdentifier);

        int msgQId;
        if ((msgQId = msgget(msgQKey, 0644)) == -1)
            ThrowMsgGetExceptions();

        AcqToProcMqBufDef message;
        TMPacketStructure tmPkt;

        int ret = 0;
        while ((ret = msgrcv(msgQId, &message, sizeof (message), 0, 0)))
        {
            if (ret == -1)
            {
                PrintMsgRcvErrors(msgQId);
                usleep(100000);
                continue;
            }

            memcpy(&tmPkt, message.OneTmFrame, DATALENGTH);

            // TODO: Remove hardcoding. Fetch from SC config API

            int tmId = (tmPkt.CortexData[10] & 0x20) >> 5;

            string identifier, tmType;

            if (tmId == 0)
                tmType = "HKTM1";
            else
                tmType = "HKTM2";

            identifier =  "CHAIN1:" + tmType;
            this->SendNewFrameToWorkerThread(identifier, tmPkt);

            // TODO: Only send packet here if it is confirmed as new. Check based on Ground Received Time
            identifier = "ALL:" + tmType;
            this->SendNewFrameToWorkerThread(identifier, tmPkt);
        }

        return SUCCESS;
    }
    catch (exception &e)
    {
        errMsg = e.what();
        return FAILURE;
    }
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
            // TODO: free defunc thread memory

            cerr << "Thread-" << td->threadId << " exited with exception: " << ex.what() << "\n";

            td->teptr = nullptr;

            thread *newThread = new thread(&ThreadSpawner::WorkerThread, this, td);
            this->processingThreads.push_back(newThread);
        }
    }

    //        Push to Internal queue
    td->packets.push(message);

    sem_post(&td->dataFilled);
}


void ThreadSpawner::ThrowMsgGetExceptions()
{
    switch (errno)
    {
    case EACCES:
        throw runtime_error("msgget: No Permission to access the queue");
        break;

    case EEXIST :
        throw runtime_error("msgget: IPC_CREAT and IPC_EXCL were specified in msgflg, but a message queue already exists for key");
        break;

    case  ENOENT:
        throw runtime_error("msgget: No message queue exists for key and msgflg did not specify IPC_CREAT");
        break;

    case ENOMEM:
        throw runtime_error("msgget: A message queue has to be created but the system does not have enough memory for the new data structure");
        break;

    case ENOSPC:
        throw runtime_error("msgget: A message queue has to be created but the system limit for the maximum number of message queues (MSGMNI) would be exceeded");
        break;

    default:
        throw  system_error(errno, generic_category(), "msgget");
        break;
    }
}


void ThreadSpawner::PrintMsgRcvErrors(int msgQId)
{
    switch (errno)
    {
    case E2BIG :
        cerr << "msgQId: [" << msgQId << "] msgrcv: The size of tmPkt is greater than expected" << endl;
        break;

    case EACCES :
        cerr << "msgQId: [" << msgQId << "] msgrcv: Operation permission is denied to the calling process" << endl;
        break;

    case EIDRM :
        cerr << "msgQId: [" << msgQId << "] msgrcv: The message queue identifier msgQId is removed from the system" << endl;
        break;

    case EINTR :
        cerr << "msgQId: [" << msgQId << "] msgrcv: Interrupted by a signal" << endl;
        break;

    case EINVAL :
        cerr << "msgQId: [" << msgQId << "] msgrcv: msgQId is not a valid message queue identifier" << endl;
        break;

    case ENOMSG :
        cerr << "msgQId: [" << msgQId << "] msgrcv: The queue does not contain a message of the desired type" << endl;
        break;

    default:
        cerr << "msgQId: [" << msgQId << "] msgrcv: " << strerror(errno) << endl;
        break;
    }
}


void ThreadSpawner::PrintPacketDetails(TMPacketStructure tmPkt)
{
    //            printf("STN_Code: %02X \n", tmPkt.StnCode);
    //            printf("DataType: %02X \n", tmPkt.DataType);
    //            printf("TMSource: %02X \n", tmPkt.TMSource);

    for (int i = 0; i < 10; i++)
        printf("%02X ", tmPkt.ReceivedTime[i]);

    cout << endl;
}
