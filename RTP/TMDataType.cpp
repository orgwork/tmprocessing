#include "TMDataType.h"


uint32_t TMDataType::getUInt32(Sample &sample)
{
    uint32_t result = 0;
    uint16_t numOfBits = 0;

    int numOfFrames = sample.frameIdList.size();
    for (int i = numOfFrames - 1 ; i >= 0 ; i--)
    {
        uint16_t frameId = sample.frameIdList.at(i);

        Address address = sample.addressList.at(i);

        uint16_t bytePos = address.wordNo;
        uint16_t startBitPos = address.startBitPos;
        uint16_t endBitPos = address.endBitPos;

        uint8_t byte = masterFrame->rawFrameMap[frameId][bytePos];
        uint8_t mask = 0x00;

        for (int bitPos = endBitPos; bitPos >= startBitPos; bitPos--)
        {
            switch (bitPos)
            {
                case 0:
                    mask = 0x80;
                    break;
                case 1:
                    mask = 0x40;
                    break;
                case 2:
                    mask = 0x20;
                    break;
                case 3:
                    mask = 0x10;
                    break;
                case 4:
                    mask = 0x08;
                    break;
                case 5:
                    mask = 0x04;
                    break;
                case 6:
                    mask = 0x02;
                    break;
                case 7:
                    mask = 0x01;
                    break;
            }

            if ( byte & mask )
                result |= ((uint32_t)0x0001 << numOfBits);

            numOfBits++;
        }

    }

    return result;
}


uint64_t TMDataType::getUInt64(Sample &sample)
{
    uint64_t result = 0;
    uint16_t numOfBits = 0;

    int numOfFrames = sample.frameIdList.size();
    for (int i = numOfFrames - 1 ; i >= 0 ; i--)
    {
        uint16_t frameId = sample.frameIdList.at(i);

        Address address = sample.addressList.at(i);

        uint16_t bytePos = address.wordNo;
        uint16_t startBitPos = address.startBitPos;
        uint16_t endBitPos = address.endBitPos;

        uint8_t byte = masterFrame->rawFrameMap[frameId][bytePos];
        uint8_t mask = 0x00;

        for (int bitPos = endBitPos; bitPos >= startBitPos; bitPos--)
        {
            switch (bitPos)
            {
                case 0:
                    mask = 0x80;
                    break;
                case 1:
                    mask = 0x40;
                    break;
                case 2:
                    mask = 0x20;
                    break;
                case 3:
                    mask = 0x10;
                    break;
                case 4:
                    mask = 0x08;
                    break;
                case 5:
                    mask = 0x04;
                    break;
                case 6:
                    mask = 0x02;
                    break;
                case 7:
                    mask = 0x01;
                    break;
            }

            if ( byte & mask )
                result |= ((uint64_t)0x0001 << numOfBits);

            numOfBits++;

        }

    }

    return result;
}


int32_t TMDataType::getInt32(Sample &sample)
{
    int32_t result = 0;
    uint16_t numOfBits = 0;

    int numOfFrames = sample.frameIdList.size();
    for (int i = numOfFrames - 1 ; i >= 0 ; i--)
    {
        uint16_t frameId = sample.frameIdList.at(i);

        Address address = sample.addressList.at(i);

        uint16_t bytePos = address.wordNo;
        uint16_t startBitPos = address.startBitPos;
        uint16_t endBitPos = address.endBitPos;

        uint8_t byte = masterFrame->rawFrameMap[frameId][bytePos];
        uint8_t mask = 0x00;

        for (int bitPos = endBitPos; bitPos >= startBitPos; bitPos--)
        {
            switch (bitPos)
            {
                case 0:
                    mask = 0x80;
                    break;
                case 1:
                    mask = 0x40;
                    break;
                case 2:
                    mask = 0x20;
                    break;
                case 3:
                    mask = 0x10;
                    break;
                case 4:
                    mask = 0x08;
                    break;
                case 5:
                    mask = 0x04;
                    break;
                case 6:
                    mask = 0x02;
                    break;
                case 7:
                    mask = 0x01;
                    break;
            }

            if ( byte & mask )
                result |= ((uint32_t)0x0001 << numOfBits);

            numOfBits++;
        }
    }

    return result;
}


int64_t TMDataType::getInt64(Sample &sample)
{
    int64_t result = 0;
    uint16_t numOfBits = 0;

    int numOfFrames = sample.frameIdList.size();
    for (int i = numOfFrames - 1 ; i >= 0 ; i--)
    {
        uint16_t frameId = sample.frameIdList[i];

        Address address = sample.addressList[i];

        uint16_t bytePos = address.wordNo;
        uint16_t startBitPos = address.startBitPos;
        uint16_t endBitPos = address.endBitPos;

        uint8_t byte = masterFrame->rawFrameMap[frameId][bytePos];
        uint8_t mask = 0x00;

        for (int bitPos = endBitPos; bitPos >= startBitPos; bitPos--)
        {
            switch (bitPos)
            {
                case 0:
                    mask = 0x80;
                    break;
                case 1:
                    mask = 0x40;
                    break;
                case 2:
                    mask = 0x20;
                    break;
                case 3:
                    mask = 0x10;
                    break;
                case 4:
                    mask = 0x08;
                    break;
                case 5:
                    mask = 0x04;
                    break;
                case 6:
                    mask = 0x02;
                    break;
                case 7:
                    mask = 0x01;
                    break;
            }

            if ( byte & mask )
                result |= ((uint64_t)0x0001 << numOfBits);

            numOfBits++;
        }
    }

    return result;
}


int64_t TMDataType::getTwosComplement(Sample &sample)
{
    uint16_t numOfBits = 0;

    int numOfFrames = sample.frameIdList.size();
    for (int i = numOfFrames - 1 ; i >= 0 ; i--)
    {
        Address address = sample.addressList.at(i);

        uint16_t startBitPos = address.startBitPos;
        uint16_t endBitPos = address.endBitPos;

        numOfBits = numOfBits + endBitPos - startBitPos + 1;
    }

    int64_t maxValue = pow(2, numOfBits);
    int64_t normalValue = getInt64(sample);

    int64_t result = (normalValue < maxValue / 2) ? normalValue : -(maxValue - normalValue);

    return result;
}


int64_t TMDataType::getOnesComplement(Sample &sample)
{
    uint16_t numOfBits = 0;

    int numOfFrames = sample.frameIdList.size();
    for (int i = numOfFrames - 1 ; i >= 0 ; i--)
    {
        Address address = sample.addressList.at(i);

        uint16_t startBitPos = address.startBitPos;
        uint16_t endBitPos = address.endBitPos;

        numOfBits = numOfBits + endBitPos - startBitPos + 1;
    }

    int64_t maxValue = pow(2, numOfBits);
    int64_t normalValue = getInt64(sample);

    int64_t result = (normalValue < maxValue / 2) ? normalValue : -((maxValue - 1) - normalValue);

    return result;
}


int64_t TMDataType::getGray2Bin(Sample &sample)
{
    int64_t n = getInt64(sample);
    int64_t result = 0;

    for (; n != 0; n = n >> 1)
        result ^= n;

    return result;
}


float TMDataType::getIEEE32Float(Sample &sample)
{
    u32f result;

    result.intValue = getUInt32(sample);

    return  result.floatValue;
}


double TMDataType::getIEEE64Float(Sample &sample)
{
    u64d result;

    result.intValue = getUInt64(sample);

    return  result.doubleValue;
}


float TMDataType::get1750A32Float(Sample &sample)
{
    int64_t mantissa = 0;
    int64_t exp = 0;

    uint32_t intbits = getUInt32(sample);

    uint16_t buffer[6] ;
    buffer[0] = uint16_t ((intbits & 0xFF000000) >> 24 );
    buffer[1] = uint16_t ((intbits & 0x00FF0000) >> 16 );
    buffer[2] = uint16_t ((intbits & 0x0000FF00) >> 8 );
    buffer[3] = uint16_t (intbits & 0x000000FF);
    buffer[4] = 0x0000;
    buffer[5] = 0x0000;

    exp = int64_t (buffer[3] & 0xFF);

    mantissa = ((int64_t) (buffer[0] & 0xFF)) << 32;
    mantissa = mantissa | (((int64_t) buffer[1] & 0xFF) << 24);
    mantissa = mantissa | (((int64_t) buffer[2] & 0xFF) << 16);
    mantissa = mantissa | (((int64_t) buffer[4] & 0xFF) << 8);
    mantissa = mantissa | ((int64_t) buffer[5] & 0xFF);

    //extending the sign bits
    if ((mantissa & 0x0000008000000000L) != 0)
        mantissa = mantissa | 0xFFFFFF0000000000L;
    else
        mantissa = mantissa & 0x000000FFFFFFFFFFL;

    //extending the sign bits
    if ((exp & 0x0000000000000080L) != 0)
        exp = exp | 0xFFFFFFFFFFFFFF00L;
    else
        exp = exp & 0x00000000000000FF;

    float result = std::pow(2.0, exp - 39) * mantissa;

    return result;
}


double TMDataType::get1750A48Float(Sample &sample)
{
    int64_t mantissa = 0;
    int64_t exp = 0;

    uint64_t longbits = getUInt64(sample);

    uint16_t buffer[6] ;
    buffer[0] = uint16_t ((longbits & 0xFF0000000000) >> 40 );
    buffer[1] = uint16_t ((longbits & 0x00FF00000000) >> 32 );
    buffer[2] = uint16_t ((longbits & 0x0000FF000000) >> 24 );
    buffer[3] = uint16_t ((longbits &  0x000000FF0000) >> 16);
    buffer[4] = uint16_t ((longbits &  0x00000000FF00) >> 8);
    buffer[5] = uint16_t (longbits &  0x0000000000FF) ;

    exp = int64_t (buffer[3] & 0xFF);

    mantissa = ((int64_t) (buffer[0] & 0xFF)) << 32;
    mantissa = mantissa | (((int64_t) buffer[1] & 0xFF) << 24);
    mantissa = mantissa | (((int64_t) buffer[2] & 0xFF) << 16);
    mantissa = mantissa | (((int64_t) buffer[4] & 0xFF) << 8);
    mantissa = mantissa | ((int64_t) buffer[5] & 0xFF);

    //extending the sign bits
    if ((mantissa & 0x0000008000000000L) != 0)
        mantissa = mantissa | 0xFFFFFF0000000000L;
    else
        mantissa = mantissa & 0x000000FFFFFFFFFFL;

    //extending the sign bits
    if ((exp & 0x0000000000000080L) != 0)
        exp = exp | 0xFFFFFFFFFFFFFF00L;
    else
        exp = exp & 0x00000000000000FF;

    double result = std::pow(2.0, exp - 39) * mantissa;

    return result;
}
