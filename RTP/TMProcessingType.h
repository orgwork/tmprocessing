#ifndef TMPROCESSINGTYPE_H
#define TMPROCESSINGTYPE_H

#include "../includes/SystemFiles.h"
#include "../includes/StringUtils.h"

enum ProcessingType
{
    PROC_1BYTE_DEC = 1,
    PROC_1BYTE_HEX = 2,
    PROC_AND_1BYTE_DEC = 3,
    PROC_AND_1BYTE_HEX = 4,
    PROC_1BYTE_POLYNOMIAL = 5,
    PROC_1BYTE_2COMPL_POLYNOMIAL = 6,
    PROC_DIGITAL = 9,
    PROC_2BYTE_DEC = 11,
    PROC_2BYTE_HEX = 12,
    PROC_AND_2BYTE_DEC = 13,
    PROC_AND_2BYTE_HEX = 14,
    PROC_2BYTE_POLYNOMIAL = 15,
    PROC_2BYTE_2COMPL_POLYNOMIAL = 16,
    PROC_AND_2BYTE_2COMPL_POLYNOMIAL = 21,
    PROC_AND_2BYTE_POLYNOMIAL = 22,
    PROC_1750_24BIT_POLYNOMIAL = 32,
    PROC_1750_16BIT_POLYNOMIAL = 33,
    PROC_1750_40BIT_POLYNOMIAL = 40,
    PROC_IEEE_48BIT_POLYNOMIAL = 46,
    PROC_IEEE_32BIT_POLYNOMIAL = 47,
    PROC_NBYTE_HEX = 52,
    PROC_32BIT_TIME = 55,
    PROC_1750_48BIT_TIME = 56,
    PROC_SPLA = 90,
    PROC_AEXP = 91,
    PROC_LUT = 92,
    PROC_CONSTANT_VARIABLE = 93,
    PROC_CPID = 96
};


class TMProcessingType
{
public:
    TMProcessingType();

    double getPolynomialValue(double *coeffs, int degree, double tmc);
    string getRadixValue(int radixBase, uint64_t tmc);

private:
    char ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
};

#endif // TMPROCESSINGTYPE_H
