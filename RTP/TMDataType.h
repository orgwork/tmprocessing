#ifndef TMDATATYPE_H
#define TMDATATYPE_H

#include "../includes/SystemFiles.h"

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


struct Byte
{
    uint8_t Pos;
    uint8_t startBitPos;
    uint8_t endBitPos;
};


struct Address
{
    vector<Byte> bytes;
};


struct Sample
{
    vector<uint8_t> frameNumbers;
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

    u_int64_t getUInt64(vector<uint8_t *> &frames, Sample &sample);

    int64_t getInt64(vector<uint8_t *> &frames, Sample &sample);

    int64_t getTwosComplement(vector<uint8_t *> &frames, Sample &sample);

    int64_t getOnesComplement(vector<uint8_t *> &frames, Sample &sample);

    int64_t getGray2Bin(vector<uint8_t *> &frames, Sample &sample);

    float getIEEE32Float(vector<uint8_t *> &frames, Sample &sample);

    double getIEEE64Float(vector<uint8_t *> &frames, Sample &sample);

    float get1750A32Float(vector<uint8_t *> &frames, Sample &sample);

    double get1750A48Float(vector<uint8_t *> &frames, Sample &sample);

private:
    int32_t getInt32(vector<uint8_t *> &frames, Sample &sample);

    u_int32_t getUInt32(vector<uint8_t *> &frames, Sample &sample);



};

#endif // TMDATATYPE_H

