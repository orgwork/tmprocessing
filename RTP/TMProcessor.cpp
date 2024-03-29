#include "TMProcessor.hpp"
#include "TMDatabase.h"
#include "ProcessParameters.h"
#include <../includes/HkTmShmBuf.h>

TMProcessor::TMProcessor()
{
    this->tmdbPtr = nullptr;
    this->TmOpDataBufDefPtr = nullptr;

    this->processParamsPtr = new ProcessParameters;
}


TMProcessor::~TMProcessor()
{
    //    ProcessParameters *processParams = (ProcessParameters *)processParamsPtr;
    //    delete processParams;
}


bool TMProcessor::InitTMProcessor(string scId, string tmFormat, string &errMsg)
{
    try
    {
        TMDatabase *tmdb = (TMDatabase *)tmdbPtr;
        ProcessParameters *processParams = (ProcessParameters *)processParamsPtr;

        this->scId = scId;
        this->tmFormat = tmFormat;

        if (tmdb == nullptr)
        {
            tmdb = new TMDatabase;
            this->tmdbPtr = tmdb;

            if (tmdb->InitDatabase(scId, tmFormat) == false)
            {
                errMsg = "Database Initialisation Failure";
                return FAILURE;
            }
        }

        processParams->tmdb = tmdb;

        return SUCCESS;
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
        return FAILURE;
    }
}


bool TMProcessor::AddParameter(string pid, string &errMsg)
{
    TMDatabase *tmdb = (TMDatabase *)tmdbPtr;
    return tmdb->LoadParameter(pid, errMsg);
}


void TMProcessor::AddAllParameters()
{
    TMDatabase *tmdb = (TMDatabase *)tmdbPtr;
    ProcessParameters *processParams = (ProcessParameters *)processParamsPtr;

    tmdb->LoadAllParameterDetails();

    if (TmOpDataBufDefPtr != nullptr)
        processParams->SetPtrTmOpDataBufDef((TmOpDataBufDef *)TmOpDataBufDefPtr);
}


bool TMProcessor::RemoveParameter(string pid, string &errMsg)
{
    TMDatabase *tmdb = (TMDatabase *)tmdbPtr;
    return tmdb->RemoveParameter(pid, errMsg);
}


void TMProcessor::RemoveAllParameters()
{
    TMDatabase *tmdb = (TMDatabase *)tmdbPtr;
    tmdb->RemoveAllParameters();
}


void TMProcessor::ProcessFrame(char *frame)
{
    ProcessParameters *processParams = (ProcessParameters *)processParamsPtr;

    processParams->ProcessFrame(frame);
}


double TMProcessor::GetProcessedRealValue(string pid)
{
    TMDatabase *tmdb = (TMDatabase *)tmdbPtr;
    ProcessParameters *processParams = (ProcessParameters *)processParamsPtr;

    double procRealValue = 0.0;
    if (tmdb->pidIndexMap.count(pid))
    {
        int pidIndex = tmdb->pidIndexMap[pid];
        procRealValue = processParams->realValue[pidIndex];
    }
    return procRealValue;
}


string TMProcessor::GetProcessedStringValue(string pid)
{
    TMDatabase *tmdb = (TMDatabase *)tmdbPtr;
    ProcessParameters *processParams = (ProcessParameters *)processParamsPtr;

    string procStringValue;
    if (tmdb->pidIndexMap.count(pid))
    {
        int pidIndex = tmdb->pidIndexMap[pid];
        procStringValue = processParams->stringValue[pidIndex];
    }
    return procStringValue;
}


uint64_t TMProcessor::GetTMRawCount(string pid)
{
    TMDatabase *tmdb = (TMDatabase *)tmdbPtr;
    ProcessParameters *processParams = (ProcessParameters *)processParamsPtr;

    uint64_t rawCount = 0;
    if (tmdb->pidIndexMap.count(pid))
    {
        int pidIndex = tmdb->pidIndexMap[pid];
        rawCount = processParams->rawValue[pidIndex];
    }
    return rawCount;
}


void TMProcessor::SetTMDatabase(void *tmdb)
{
    this->tmdbPtr = tmdb;
}


void TMProcessor::SetSharedMemoryPointer(void *ptr)
{
    this->TmOpDataBufDefPtr = ptr;
}
