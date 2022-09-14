#ifndef PARAMETERINFO_H
#define PARAMETERINFO_H


#include "../includes/SystemFiles.h"
#include "TMDataType.h"
#include "TMProcessingType.h"

enum ConditionType
{
    OnOtherParams = 1,
    OnSingleValue = 2,
    OnValueRange  = 3
};


enum ParameterType
{
    POLYNOMIAL,
    DIGITAL,
    RADIX,
    LUT,
    NONE
};


enum DisplayFormat
{
    DECIMAL = 10,
    HEXADECIMAL = 16,
};


class ParameterInfo
{

public:
    ParameterInfo();

    int16_t          pidIndex;
    string cdbPid;
    string mnemonic;
    ProcessingType   procType;
    DataType         dataType;
    ParameterType    paramType;
    DisplayFormat    displayFormat;
    bool             isDerivedParam;
    bool             isConditional;
    ConditionType    condType;
    map<int, string> digitalStatusMap;
    int          polynomialDegree;
    double_t         *coefficientList; // 0 - x^0 = 1 ie, Constant // 1 - x // 2 - x^2  // 3 - x^3

    Sample frameIdSamplesMap[32];

};

#endif // PARAMETERINFO_H
