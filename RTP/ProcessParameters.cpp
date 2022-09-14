#include "ProcessParameters.h"

ProcessParameters::ProcessParameters()
{
    this->masterFrame = new MasterFrame;

    tmDataType.masterFrame = this->masterFrame;
    ptrTmOpDataBufDef = nullptr;
    for (int i = 0; i < MAXPID; i++)
    {
        rawValue[i] = 0;
        realValue[i] = 0.0;
    }
}


void ProcessParameters::ProcessFrame(char *rawFrame)
{
    this->curFrameId =  masterFrame->updateFrame(rawFrame);

    vector<int> pidIndexList = tmdb->frameIdPidIndexListMap[curFrameId];

    int pidIndexListSize = pidIndexList.size();

    for (int indexNo = 0; indexNo < pidIndexListSize; indexNo++)
    {
        int pidIndex = pidIndexList.at(indexNo);

        ParameterInfo *parameter = tmdb->pidIndexParameterMap[pidIndex];

        UpdateRawValue(parameter);

        UpdateProcessedValue(parameter);

        if (ptrTmOpDataBufDef != nullptr)
        {
            this->ptrTmOpDataBufDef->HkTmPrcOpBuf[pidIndex].TmCount = rawValue[pidIndex];
            this->ptrTmOpDataBufDef->HkTmPrcOpBuf[pidIndex].RealValue = realValue[pidIndex];
            this->ptrTmOpDataBufDef->HkTmPrcOpBuf[pidIndex].StringValue[MAXCHARVALFORPID] = '\0';
            strncpy(this->ptrTmOpDataBufDef->HkTmPrcOpBuf[pidIndex].StringValue, stringValue[pidIndex].c_str(), MAXCHARVALFORPID);
        }
    }

}


void ProcessParameters::UpdateRawValue(ParameterInfo *parameter)
{
    int pidIndex = parameter->pidIndex;
    switch (parameter->dataType)
    {
        case DataType::Normal:
            rawValue[pidIndex] = tmDataType.getInt64(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::OnesComplement:
            rawValue[pidIndex] =  tmDataType.getOnesComplement(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::TwosCompliment:
            rawValue[pidIndex] =  tmDataType.getTwosComplement(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::GrayCode:
            rawValue[pidIndex] =  tmDataType.getGray2Bin(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::FloatIEEE32Bit:
            rawValue[pidIndex] =  tmDataType.getIEEE32Float(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::FloatIEEE64Bit:
            rawValue[pidIndex] =  tmDataType.getIEEE64Float(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::Float1750A32Bit:
            rawValue[pidIndex] =  tmDataType.get1750A32Float(parameter->frameIdSamplesMap[curFrameId]);
            break;

        case DataType::Float1750A48Bit:
            rawValue[pidIndex] =  tmDataType.get1750A48Float(parameter->frameIdSamplesMap[curFrameId]);
            break;
    }
}


void ProcessParameters::UpdateProcessedValue(ParameterInfo *parameter)
{
    int pidIndex = parameter->pidIndex;
    switch (parameter->paramType)
    {
        case ParameterType::DIGITAL:
            {
                int key = rawValue[pidIndex];
                realValue[pidIndex] = key;
                stringValue[pidIndex] = parameter->digitalStatusMap[key];
                break;
            }

        case ParameterType::POLYNOMIAL:
            {
                realValue[pidIndex] = tmProcType.getPolynomialValue(parameter->coefficientList, parameter->polynomialDegree, rawValue[pidIndex]);
                stringValue[pidIndex] = to_string(realValue[pidIndex]);
                break;
            }

        case ParameterType::RADIX:
            {
                realValue[pidIndex] = rawValue[pidIndex];
                stringValue[pidIndex] = tmProcType.getRadixValue(parameter->displayFormat, rawValue[pidIndex]);
                break;
            }
        case ParameterType::LUT:

            break;

        default:
            break;
    }
}
