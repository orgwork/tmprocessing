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



string TMProcessingType::getRadixValue(int radixBase, uint64_t tmc)
{
    string result;

    char ascii[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int i = 0, j, cnt;
    char res1[20], res[20];
    do
    {
        res1[i++] = ascii[(tmc % radixBase)];
        tmc = (tmc / radixBase);
    }
    while (tmc != 0);
    res1[i] = '\0';
    cnt = i;
    for (i--, j = 0; j < cnt; j++, i--)
        res[j] = res1[i];
    res[j] = '\0';

    result = res;

    return result;
}
