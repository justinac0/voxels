#ifndef VMATH_TYPE_H
#define VMATH_TYPE_H

#include <stddef.h>
#include <math.h>

#ifdef VMATH_HIGH_PREC
    typedef float real;
    #define sqrtr sqrt
#endif

#ifndef VMATH_LOW_PREC
    #ifndef VMATH_HIGH_PREC
        #define VMATH_LOW_PREC
    #endif
#endif

#ifdef VMATH_LOW_PREC
    typedef float real;
    #define sqrtr sqrtf
#endif

#endif // VMATH_TYPE_H