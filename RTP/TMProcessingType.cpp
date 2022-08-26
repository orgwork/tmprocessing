#include "TMProcessingType.h"

TMProcessingType::TMProcessingType()
{

}


double TMProcessingType::getPolynomialValue(double *coeffs, uint8_t degree, double tmc)
{
    double result = 0.0;
    double factor = 1;

    for (uint8_t exp = 0; exp <= degree; exp++)
    {
        result += coeffs[exp] * factor;
        factor *= tmc;
    }

    return result;
}



string TMProcessingType::getRadixValue(int radixBase, double tmc)
{
    string result;
    stringstream stream;

    switch (radixBase)
    {
    case 16:
        stream << std::hex << tmc;
        result = stream.str();
        break;

    case 10:
        stream << std::dec << tmc;
        result = stream.str();
        break;

    case 8:
        stream << std::oct << tmc;
        result = stream.str();
        break;

    case 2:
        result = bitset<64>(tmc).to_string();
        break;
    }

    return result;
}
