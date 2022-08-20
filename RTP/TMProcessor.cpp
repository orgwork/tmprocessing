#include "TMProcessor.hpp"

TMProcessor::TMProcessor(string satId, string tmFormat)
{
    this->satId = satId;
    this->tmFormat = tmFormat;
}


bool TMProcessor::InitTMProcessor(string &errMsg)
{

    return SUCCESS;
}


bool TMProcessor::AddParameter(string pid, string &errMsg)
{

    return SUCCESS;
}


bool TMProcessor::RemoveParameter(string pid, string &errMsg)
{

    return SUCCESS;
}


bool TMProcessor::ProcessFrame(char *frame)
{

    return SUCCESS;
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
