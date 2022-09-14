#include "TMDatabase.h"


TMDatabase::TMDatabase()
{
    this->tmDBReader = new TMDBReader;
}


bool TMDatabase::InitDatabase(string scId, string tmFormatId)
{
    this->tmFormatId = getTMFormatId(tmFormatId);

    // TODO: For offline database loading, whether to use shared memory or file reading

    if (tmDBReader->init((char *)scId.c_str(), 0) == false)
        return FAILURE;

    return SUCCESS;
}


TMFormatId TMDatabase::getTMFormatId(string tmFormatId)
{
    if (tmFormatId == "HKTM") return NormalTM;
    else if (tmFormatId == "CTM") return CTM;
    else if (tmFormatId == "BTM") return BTM;
    else if (tmFormatId == "F2PB") return F2PB;
    else if (tmFormatId == "F3PB") return F3PB;
    else if (tmFormatId == "SPS") return SPS;
    else return InvalidTM;
}


bool TMDatabase::LoadParameter(string paramId, string &errorMsg)
{
    try
    {
        bool     retSts = false;
        uint8_t errMsg[30];
        // TODO: UPDATE FROM SC CONFIG
        int numOfSubFrames = 32;

        ParameterInfo *parameter = new ParameterInfo;

        uint8_t *pid = (uint8_t *)paramId.c_str();

        parameter->pidIndex = tmDBReader->glbTMDB_ptr->getPIDIndex(pid, &retSts, errMsg);

        if (retSts == false)
        {
            errorMsg = (char *)errMsg;
            return FAILURE;
        }

        this->pidIndexMap[paramId] = parameter->pidIndex;

        parameter->cdbPid = paramId;
        parameter->mnemonic = (char *)tmDBReader->glbTMDB_ptr->getPIDMnemonic(pid, &retSts, errMsg);
        parameter->dataType = (DataType)tmDBReader->glbTMDB_ptr->getPIDDataType(pid, &retSts, errMsg);
        parameter->procType = (ProcessingType)tmDBReader->glbTMDB_ptr->getPIDProcType(pid, &retSts, errMsg);

        uint8_t noOfConds = tmDBReader->glbTMDB_ptr->getPIDNoOfCondn(pid, &retSts, errMsg);

        uint8_t noOfSets = tmDBReader->glbTMDB_ptr->getPIDNoOfSets(pid, &retSts, errMsg);

        vector<int> updateFrames;

        // TODO: HANDLE MORE THAN ONE SETS
        for (uint8_t setNo = 0; setNo < noOfSets; setNo++)
        {
            uint8_t   noOfChns      = tmDBReader->glbTMDB_ptr->getPIDNoOfChn(pid, setNo, &retSts, errMsg);
            uint16_t *samplingRates = tmDBReader->glbTMDB_ptr->getPIDSamplingRate(pid, setNo, &retSts, errMsg);
            uint16_t *words         = tmDBReader->glbTMDB_ptr->getPIDListOfWords(pid, setNo, &retSts, errMsg);
            uint16_t *frames        = tmDBReader->glbTMDB_ptr->getPIDListOfFrames(pid, setNo, &retSts, errMsg);
            uint16_t *startBits     = tmDBReader->glbTMDB_ptr->getPIDListOfStartBits(pid, setNo, &retSts, errMsg);
            uint16_t *endBits       = tmDBReader->glbTMDB_ptr->getPIDListOfEndBits(pid, setNo, &retSts, errMsg);

            int maxSamplingRate = 0;

            for (uint8_t chnSet = 0; chnSet < noOfChns; chnSet++)
            {
                uint16_t samplingRate = samplingRates[chnSet];

                if (samplingRate == 0)
                    samplingRate = 1;

                if (samplingRate > maxSamplingRate)
                    maxSamplingRate = samplingRate;
            }

            vector<uint8_t> frameNumbers[noOfChns];
            for (uint8_t chnSet = 0; chnSet < noOfChns; chnSet++)
            {
                uint16_t samplingRate = samplingRates[chnSet];

                if (samplingRate == 0)
                    samplingRate = 1;

                uint16_t startFrame = frames[chnSet];

                for (uint16_t frame = startFrame; frame < numOfSubFrames; frame = frame + numOfSubFrames / samplingRate)
                {
                    int repetition = maxSamplingRate / samplingRate;

                    for (int repeatNo = 0; repeatNo < repetition; repeatNo++)
                        frameNumbers[chnSet].push_back(frame);
                }
            }


            for (uint8_t sampleNo = 0; sampleNo < maxSamplingRate; sampleNo++)
            {
                Sample sample;
                for (uint8_t chnSet = 0; chnSet < noOfChns; chnSet++)
                {
                    Address address;

                    address.frameId = frameNumbers[chnSet].at(sampleNo);
                    address.wordNo = words[chnSet];
                    address.startBitPos = startBits[chnSet];
                    address.endBitPos = endBits[chnSet];

                    sample.addressList.push_back(address);
                    sample.frameIdList.push_back(frameNumbers[chnSet].at(sampleNo));
                }
                uint16_t updateFrameNo = frameNumbers[noOfChns - 1].at(sampleNo);
                updateFrames.push_back(updateFrameNo);

                parameter->frameIdSamplesMap[updateFrameNo] = sample;
            }

            switch (parameter->procType)
            {
                // ********** DIGITAL Parameters **********
                case ProcessingType::PROC_DIGITAL:
                    {
                        parameter->paramType = ParameterType::DIGITAL;

                        int noOfDigitalMsgs = tmDBReader->glbTMDB_ptr->getPIDNoOfDigMsg(pid, setNo, &retSts, errMsg);

                        // TODO: REMOVE AFTER DB CORRECTION
                        if (noOfDigitalMsgs > 50)
                            break;

                        uint8_t     *keys = tmDBReader->glbTMDB_ptr->getPIDListOfDigPtnKeys(pid, setNo, &retSts, errMsg);
                        vector<uint8_t> keyList(keys, keys + noOfDigitalMsgs);

                        char         **msgs = (char **)tmDBReader->glbTMDB_ptr->getPIDListOfDigMsg(pid, setNo, &retSts, errMsg);
                        vector<string> msgList(msgs, msgs + noOfDigitalMsgs);

                        for (int msgNo = 0; msgNo < noOfDigitalMsgs; msgNo++)
                        {
                            int    key   = keyList.at(msgNo);
                            string value = msgList.at(msgNo);

                            parameter->digitalStatusMap[key] = value;
                        }

                        break;
                    }

                // ********** RADIX Parameters - DEC **********
                case ProcessingType::PROC_1BYTE_DEC:
                case ProcessingType::PROC_2BYTE_DEC:
                case ProcessingType::PROC_AND_1BYTE_DEC:
                case ProcessingType::PROC_AND_2BYTE_DEC:
                    {
                        parameter->paramType = ParameterType::RADIX;

                        parameter->displayFormat = DisplayFormat::DECIMAL;

                        break;
                    }

                // ********** RADIX Parameters - HEX **********
                case ProcessingType::PROC_1BYTE_HEX:
                case ProcessingType::PROC_2BYTE_HEX:
                case ProcessingType::PROC_NBYTE_HEX:
                case ProcessingType::PROC_AND_1BYTE_HEX:
                case ProcessingType::PROC_AND_2BYTE_HEX:
                    {
                        parameter->paramType = ParameterType::RADIX;

                        parameter->displayFormat = DisplayFormat::HEXADECIMAL;

                        break;
                    }

                // ********** POLYNOMIALicient Parameters **********
                case ProcessingType::PROC_1BYTE_POLYNOMIAL:
                case ProcessingType::PROC_2BYTE_POLYNOMIAL:
                case ProcessingType::PROC_AND_2BYTE_POLYNOMIAL:
                case ProcessingType::PROC_1750_16BIT_POLYNOMIAL:
                case ProcessingType::PROC_1750_24BIT_POLYNOMIAL:
                case ProcessingType::PROC_1750_40BIT_POLYNOMIAL:
                case ProcessingType::PROC_IEEE_32BIT_POLYNOMIAL:
                case ProcessingType::PROC_IEEE_48BIT_POLYNOMIAL:
                case ProcessingType::PROC_1BYTE_2COMPL_POLYNOMIAL:
                case ProcessingType::PROC_2BYTE_2COMPL_POLYNOMIAL:
                case ProcessingType::PROC_AND_2BYTE_2COMPL_POLYNOMIAL:
                    {
                        parameter->paramType = ParameterType::POLYNOMIAL;

                        int degree = tmDBReader->glbTMDB_ptr->getPIDEqnOrder(pid, setNo, &retSts, errMsg);

                        // TODO: REMOVE AFTER DB CORRECTION
                        if (degree > 10)
                            break;

                        parameter->polynomialDegree = degree;
                        parameter->coefficientList  = tmDBReader->glbTMDB_ptr->getPIDEqnCoeffOffset(pid, setNo, &retSts, errMsg);

                        break;
                    }

                case ProcessingType::PROC_1750_48BIT_TIME:
                case ProcessingType::PROC_32BIT_TIME:
                case ProcessingType::PROC_AEXP:
                case ProcessingType::PROC_CONSTANT_VARIABLE:
                case ProcessingType::PROC_CPID:
                case ProcessingType::PROC_LUT:
                case ProcessingType::PROC_SPLA:

                    parameter->paramType = ParameterType::NONE;
                    break;


                default:
                    parameter->paramType = ParameterType::NONE;

                    break;
            }
        }

        pidIndexParameterMap[parameter->pidIndex] = parameter;

        for (size_t i = 0; i < updateFrames.size(); i++)
        {
            int updateFrameNo = updateFrames.at(i);
            //            cout << updateFrameNo << endl;

            frameIdPidIndexListMap[updateFrameNo].push_back(parameter->pidIndex);
        }

        return SUCCESS;
    }
    catch (exception &e)
    {
        errorMsg = e.what();
        return FAILURE;
    }
}


void TMDatabase::LoadAllParameterDetails()
{
    try
    {
        bool     retSts = false;
        uint8_t errMsg[30];
        string errorMsg;

        int            numOfPids = tmDBReader->glbTMDB_ptr->getTotalNumPids(tmFormatId, &retSts, errMsg);
        char         **pids      = (char **)tmDBReader->glbTMDB_ptr->getPIDList(tmFormatId, &retSts, errMsg);
        vector<string> pidVector(pids, pids + numOfPids);

        for (size_t i = 0; i < pidVector.size(); i++)
        {
            this->LoadParameter(pidVector.at(i), errorMsg);
        }
    }
    catch (std::exception &e)
    {
        cout << e.what() << endl;
    }
}


bool TMDatabase::RemoveParameter(string paramId, string &errorMsg)
{
    // TODO: Remove from frameIdPidIndexListMap & delete pidIndexParameterMap
    if (pidIndexMap.count(paramId))
    {
        pidIndexMap.erase(paramId);
        return SUCCESS;
    }
    else
    {
        errorMsg = "INVALID PID";
        return FAILURE;
    }
}


void TMDatabase::RemoveAllParameters()
{
    this->pidIndexMap.clear();
}
