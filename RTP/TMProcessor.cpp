#include "TMProcessor.hpp"

TMProcessor::TMProcessor(string satId, string tmFormat)
{
    this->satId = satId;
    this->tmFormat = tmFormat;
    this->tmdb = nullptr;

    this->processParams = new ProcessParameters;
}


void TMProcessor::SetTMDatabase(TMDatabase *tmdb)
{
    this->tmdb = tmdb;
}


bool TMProcessor::InitTMProcessor(string &errMsg)
{
    if (this->tmdb == nullptr)
    {
        if (tmdb->InitDatabase(satId, tmFormat) == false)
        {
            errMsg = "Database Initialisation Failure";
            return FAILURE;
        }
    }

    this->processParams->tmdb = tmdb;
    this->processParams->ptrTmOpDataBufDef = ptrTmOpDataBufDef;

    return SUCCESS;
}


bool TMProcessor::AddParameter(string pid, string &errMsg)
{

    return SUCCESS;
}


void TMProcessor::AddAllParameters()
{
    this->tmdb->LoadAllParameterDetails();
}


bool TMProcessor::RemoveParameter(string pid, string &errMsg)
{

    return SUCCESS;
}


void TMProcessor::RemoveAllParameters()
{

}


void TMProcessor::ProcessFrame(char *frame)
{
    this->processParams->ProcessFrame(frame);
}


double TMProcessor::GetProcessedRealValue(string pid)
{
    double procValue = 0.0;

    return procValue;
}


string TMProcessor::GetProcessedStringValue(string pid)
{
    string procValue;

    return procValue;
}


uint64_t TMProcessor::GetTMRawCount(string pid)
{
    uint64_t rawCount = 0;

    return rawCount;
}


void TMProcessor::SetSharedMemoryPointer(TmOpDataBufDef *ptr)
{
    this->ptrTmOpDataBufDef = ptr;
}
