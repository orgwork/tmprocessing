#ifndef TMDATATYPE_H
#define TMDATATYPE_H

#include "../includes/SystemFiles.h"
#include "MasterFrame.h"

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


struct Address
{
    uint16_t frameId;
    uint16_t wordNo; // starts from 0
    uint8_t startBitPos;
    uint8_t endBitPos;
};

struct Sample
{
    vector<uint16_t> frameIdList;
    vector<Address> addressList;
};


union u32f
{
    uint32_t intValue;
    float floatValue;
};


union u64d
{
    uint64_t intValue;
    double doubleValue;
};


class TMDataType
{

public:

    u_int64_t getUInt64(Sample &sample);

    int64_t getInt64(Sample &sample);

    int64_t getTwosComplement(Sample &sample);

    int64_t getOnesComplement(Sample &sample);

    int64_t getGray2Bin(Sample &sample);

    float getIEEE32Float(Sample &sample);

    double getIEEE64Float(Sample &sample);

    float get1750A32Float(Sample &sample);

    double get1750A48Float(Sample &sample);

    MasterFrame *masterFrame;

private:
    int32_t getInt32(Sample &sample);

    u_int32_t getUInt32(Sample &sample);

};

#endif // TMDATATYPE_H

