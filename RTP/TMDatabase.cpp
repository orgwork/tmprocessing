#include "TMDatabase.h"


TMDatabase::TMDatabase()
{
    this->tmDBReader = new TMDBReader;
}


bool TMDatabase::InitDatabase(string scId, string tmFormatId)
{
    auto start = chrono::high_resolution_clock::now();

    this->tmFormatId = getTMFormatId(tmFormatId);

    // TODO: FETCH FROM ENV VAR
    char filename[100];
    strcpy(filename, "/home/pramod/data/ParamSaveFile.OCN-03.bin");
    tmDBReader->setBinFileName(filename);

    scId = "Ocn3";

    if (tmDBReader->init((char *)scId.c_str(), 1) == false)
        return FAILURE;

    bool retSts = false;
    uint8_t *errMsg = nullptr;

    int16_t numOfPids = tmDBReader->glbTMDB_ptr->getTotalNumPids(this->tmFormatId, &retSts, errMsg);
    if (retSts == true)
        cout << "\nTotal Number of Pids: " << numOfPids << endl;
    else
        cout << errMsg << endl;


    this->LoadAllParameterDetails();

    auto end = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    return SUCCESS;
}


TMFormatId TMDatabase::getTMFormatId(string tmFormatId)
{
    if (tmFormatId == "HKTM")               return NormalTM;
    else if (tmFormatId == "CTM")           return CTM;
    else if (tmFormatId == "BTM")           return BTM;
    else if (tmFormatId == "F2PB")          return F2PB;
    else if (tmFormatId == "F3PB")          return F3PB;
    else if (tmFormatId == "SPS")           return SPS;
    else                                    return InvalidTM;
}


bool TMDatabase::LoadParameter(string pid)
{

    return SUCCESS;
}


void TMDatabase::LoadAllParameterDetails()
{
    try
    {
        int paramcount = 0;
        bool retSts = false;
        uint8_t *errMsg = nullptr;

        // TODO: UPDATE FROM SC CONFIG
        int numOfSubFrames = 32;

        int numOfPids = tmDBReader->glbTMDB_ptr->getTotalNumPids(tmFormatId, &retSts, errMsg);

        for (int frameNo = 0; frameNo < numOfSubFrames; frameNo++)
        {
            char **pids = (char **)tmDBReader->glbTMDB_ptr->getPidsInCurFrm(frameNo, tmFormatId, &retSts, errMsg);
            int numOfPids = tmDBReader->glbTMDB_ptr->getNumPidsInCurFrm(frameNo, tmFormatId, &retSts, errMsg);

            vector<string> pidVector(pids, pids + numOfPids);

            vector<ParameterInfo> parameterList;

            for (size_t i = 0; i < pidVector.size(); i++)
            {
                paramcount++;
                ParameterInfo parameter;

                uint8_t *pid = (uint8_t *)pidVector.at(i).c_str();

                parameter.cdbPid = (char *)pid;
                parameter.pidIndex = tmDBReader->glbTMDB_ptr->getPIDIndex(pid, &retSts, errMsg);
                parameter.mnemonic = (char *)tmDBReader->glbTMDB_ptr->getPIDMnemonic(pid, &retSts, errMsg);
                parameter.dataType = (DataType)tmDBReader->glbTMDB_ptr->getPIDDataType(pid, &retSts, errMsg);
                parameter.procType = (ProcessingType)tmDBReader->glbTMDB_ptr->getPIDProcType(pid, &retSts, errMsg);

                //                uint8_t getPIDNoOfChn(uint8_t *PID, uint8_t setNo, bool * retSts, uint8_t *errMsg);
                //                uint16_t *getPIDSamplingRate(uint8_t *PID, uint8_t setNo, bool * retSts, uint8_t *errMsg);
                //                uint16_t *getPIDListOfWords(uint8_t *PID, uint8_t  setNo, bool * retSts, uint8_t *errMsg);
                //                uint16_t *getPIDListOfFrames(uint8_t *PID, uint8_t setNo, bool * retSts, uint8_t *errMsg);
                //                uint16_t *getPIDListOfStartBits(uint8_t *PID, uint8_t setNo, bool * retSts, uint8_t *errMsg);
                //                uint16_t *getPIDListOfEndBits(uint8_t *PID, uint8_t setNo, bool * retSts, uint8_t *errMsg);

                int noOfSets = tmDBReader->glbTMDB_ptr->getPIDNoOfSets(pid, &retSts, errMsg);

                cout << frameNo << "\t" << parameter.mnemonic << endl;

                // TODO: HANDLE MORE THAN ONE SETS
                for (int setNo = 0; setNo < noOfSets; setNo++)
                {
                    uint8_t noOfChns = tmDBReader->glbTMDB_ptr->getPIDNoOfChn(pid, setNo, &retSts, errMsg);
                    uint16_t *samplingRates = tmDBReader->glbTMDB_ptr->getPIDSamplingRate(pid, setNo, &retSts, errMsg);
                    uint16_t *words = tmDBReader->glbTMDB_ptr->getPIDListOfWords(pid, setNo, &retSts, errMsg);
                    uint16_t *frames = tmDBReader->glbTMDB_ptr->getPIDListOfFrames(pid, setNo, &retSts, errMsg);
                    uint16_t *startBits = tmDBReader->glbTMDB_ptr->getPIDListOfStartBits(pid, setNo, &retSts, errMsg);
                    uint16_t *endBits = tmDBReader->glbTMDB_ptr->getPIDListOfEndBits(pid, setNo, &retSts, errMsg);

                    //                    cout << "********* " << (unsigned)noOfChns << endl;


                    //                    for (uint8_t chnSet = 0; chnSet < noOfChns; chnSet++)
                    //                    {
                    //                        cout << words[chnSet] << endl;
                    //                    }


                    switch (parameter.procType)
                    {

                        // ********** DIGITAL Parameters **********
                        case ProcessingType::PROC_DIGITAL:
                            {
                                parameter.paramType = ParameterType::DIGITAL;

                                int noOfDigitalMsgs = tmDBReader->glbTMDB_ptr->getPIDNoOfDigMsg(pid, setNo, &retSts, errMsg);

                                // TODO: REMOVE AFTER DB CORRECTION
                                if (noOfDigitalMsgs > 50)
                                    break;

                                uint8_t *keys = tmDBReader->glbTMDB_ptr->getPIDListOfDigPtnKeys(pid, setNo, &retSts, errMsg);
                                vector<uint8_t> keyList(keys, keys + noOfDigitalMsgs);

                                char **msgs = (char **)tmDBReader->glbTMDB_ptr->getPIDListOfDigMsg(pid, setNo, &retSts, errMsg);
                                vector<string> msgList(msgs, msgs + noOfDigitalMsgs);

                                for (int msgNo = 0; msgNo < noOfDigitalMsgs; msgNo++)
                                {
                                    int key = keyList.at(msgNo);
                                    string value = msgList.at(msgNo);

                                    parameter.digitalStatusMap[key] = value;
                                }

                                break;
                            }


                        // ********** RADIX Parameters - DEC **********
                        case ProcessingType::PROC_1BYTE_DEC:
                        case ProcessingType::PROC_2BYTE_DEC:
                        case ProcessingType::PROC_AND_1BYTE_DEC:
                        case ProcessingType::PROC_AND_2BYTE_DEC:
                            {
                                parameter.paramType = ParameterType::RADIX;

                                parameter.displayFormat = DisplayFormat::DECIMAL;

                                break;
                            }


                        // ********** RADIX Parameters - HEX **********
                        case ProcessingType::PROC_1BYTE_HEX:
                        case ProcessingType::PROC_2BYTE_HEX:
                        case ProcessingType::PROC_NBYTE_HEX:
                        case ProcessingType::PROC_AND_1BYTE_HEX:
                        case ProcessingType::PROC_AND_2BYTE_HEX:
                            {
                                parameter.paramType = ParameterType::RADIX;

                                parameter.displayFormat = DisplayFormat::HEXADECIMAL;

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
                                parameter.paramType = ParameterType::POLYNOMIAL;

                                int degree = tmDBReader->glbTMDB_ptr->getPIDEqnOrder(pid, setNo, &retSts, errMsg);

                                // TODO: REMOVE AFTER DB CORRECTION
                                if (degree > 10)
                                    break;

                                parameter.polynomialDegree = degree;
                                parameter.coefficientList = tmDBReader->glbTMDB_ptr->getPIDEqnCoeffOffset(pid, setNo, &retSts, errMsg);

                                break;
                            }
                    }
                }


                parameterList.push_back(parameter);
                pidIndexMap[parameter.cdbPid] = parameter.pidIndex;
            }

            framePidsMap[frameNo] = parameterList;
        }

        cout << paramcount << endl;
    }
    catch (std::exception &e)
    {
        cout << e.what() << endl;
    }

}

