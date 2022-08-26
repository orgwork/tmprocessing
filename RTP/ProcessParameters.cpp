#include "ProcessParameters.h"

ProcessParameters::ProcessParameters()
{
    this->masterFrame = new MasterFrame;

    tmDataType.masterFrame = this->masterFrame;
}


void ProcessParameters::ProcessFrame(char *rawFrame)
{
    auto startTime = chrono::high_resolution_clock::now();

    this->curFrameId =  masterFrame->updateFrame(rawFrame);

    vector<int> pidIndexList = tmdb->frameIdPidIndexListMap[curFrameId];

    int pidIndexListSize = pidIndexList.size();

    for (int indexNo = 0; indexNo < pidIndexListSize; indexNo++)
    {
        int pidIndex = pidIndexList.at(indexNo);

        ParameterInfo *parameter = tmdb->pidIndexParameterMap[pidIndex];

        this->ProcessParameter(parameter);
    }

    auto endTime = chrono::high_resolution_clock::now();

    cout << "Processed Frame: " << curFrameId << " with " << pidIndexListSize << " parameters in " << chrono::duration_cast<chrono::microseconds>(endTime - startTime).count() << " us" << endl;
}


void ProcessParameters::ProcessParameter(ParameterInfo *parameter)
{
    UpdateRawValue(parameter);

    if (parameter->pidIndex != 6079)
        UpdateProcessedValue(parameter);

    this->ptrTmOpDataBufDef->HkTmPrcOpBuf[parameter->pidIndex].TmCount = parameter->rawValue;
    this->ptrTmOpDataBufDef->HkTmPrcOpBuf[parameter->pidIndex].RealValue = parameter->realValue;
    strncpy(this->ptrTmOpDataBufDef->HkTmPrcOpBuf[parameter->pidIndex].StringValue, parameter->stringValue.c_str(), MAXCHARVALFORPID);
}


void ProcessParameters::UpdateRawValue(ParameterInfo *parameter)
{
    switch (parameter->dataType)
    {
        case DataType::Normal:
            parameter->rawValue = tmDataType.getInt64(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::OnesComplement:
            parameter->rawValue =  tmDataType.getOnesComplement(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::TwosCompliment:
            parameter->rawValue =  tmDataType.getTwosComplement(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::GrayCode:
            parameter->rawValue =  tmDataType.getGray2Bin(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::FloatIEEE32Bit:
            parameter->rawValue =  tmDataType.getIEEE32Float(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::FloatIEEE64Bit:
            parameter->rawValue =  tmDataType.getIEEE64Float(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::Float1750A32Bit:
            parameter->rawValue =  tmDataType.get1750A32Float(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::Float1750A48Bit:
            parameter->rawValue =  tmDataType.get1750A48Float(parameter->frameIdSamplesMap[curFrameId]);
            break;
    }
}


void ProcessParameters::UpdateProcessedValue(ParameterInfo *parameter)
{
    switch (parameter->paramType)
    {
        case ParameterType::DIGITAL:
            {
                int key = parameter->rawValue;
                parameter->realValue = key;
                parameter->stringValue = parameter->digitalStatusMap[key];
                break;
            }

        case ParameterType::POLYNOMIAL:
            {
                parameter->realValue = tmProcType.getPolynomialValue(parameter->coefficientList, parameter->polynomialDegree, parameter->rawValue);
                parameter->stringValue = to_string(parameter->realValue);
                break;
            }

        case ParameterType::RADIX:
            {
                parameter->realValue = parameter->rawValue;
                parameter->stringValue = tmProcType.getRadixValue(parameter->displayFormat, parameter->rawValue);
                break;
            }
        case ParameterType::LUT:

            break;

        default:
            break;
    }
}
