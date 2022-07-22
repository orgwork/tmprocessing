#ifndef PARAMETERINFO_H
#define PARAMETERINFO_H


#include "includes/SystemFiles.h"


class ParameterInfo
{
    enum ConditionType
    {
        OnOtherParams = 1,
        OnSingleValue = 2,
        OnValueRange = 3
    };

    enum ProcessingType
    {

    };

    enum DataType
    {
        Normal = 1,
        OnesComplement = 2,
        TwosCompliment = 3,
        GrayCode = 4,
        FloatIEEE32Bit = 5,
        FloatIEEE64Bit = 6,
        Float1750A32Bit = 7,
        Float1750A48Bit = 8
    };

    enum DisplayType
    {

    };


public:
    ParameterInfo();


private:
    string pid;
    bool isDerivedParam;
    bool isConditional;
    ConditionType condType;
    ProcessingType procType;
    DataType dataType;
    DisplayType displayType;
    map<int, string> digitalStatusMap;
    map<int, double> coefficientsMap;   // 0 - x^0 = 1 ie, Constant // 1 - x // 2 - x^2  // 3 - x^3
};

#endif // PARAMETERINFO_H
