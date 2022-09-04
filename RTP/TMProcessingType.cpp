#include "TMProcessingType.h"

TMProcessingType::TMProcessingType()
{

}


double TMProcessingType::getPolynomialValue(double *coeffs, int degree, double tmc)
{
    double result = 0.0;
    double factor = 1;
    for (int exp = 0; exp < degree; exp++)
    {
        result += coeffs[exp] * factor;
        factor *= tmc;
    }

    return result;
}



string TMProcessingType::getRadixValue(int radixBase, uint64_t tmc)
{
    string result;

    int i = 0, j, cnt;
    char res1[17], res[17];

    do
    {
        res1[i++] = ascii[(tmc % radixBase)];
        tmc = (tmc / radixBase);
    }
    while (tmc != 0);

    cnt = i;
    for (i--, j = 0; j < cnt; j++, i--)
        res[j] = res1[i];

    res[j] = '\0';

    result = res;

    return result;
}
