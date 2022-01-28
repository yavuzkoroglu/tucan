/** \file doublecmp.c
 ** \brief Implements the double comparison functions in doublecmp.h
 **/
#include "doublecmp.h"

int areEqual_dbl(const double x, const double y) { return (y - DBL_EPSILON < x && x < y + DBL_EPSILON); }
int geq_dbl(const double x, const double y) { return x > y || areEqual_dbl(x, y); }
