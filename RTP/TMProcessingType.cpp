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


