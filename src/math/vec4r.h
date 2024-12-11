#ifndef VEC4R_H
#define VEC4R_H

#include "type.h"

typedef union Vec4r {
    real v[4];
    struct {
        real x, y, z, w;
    };
    struct {
        real r, g, b, a;
    };
} Vec4r;

Vec4r vec4r_add(Vec4r* a, Vec4r* b);
Vec4r vec4r_sub(Vec4r* a, Vec4r* b);
Vec4r vec4r_mul(Vec4r* a, Vec4r* b);
real vec4r_dot(Vec4r* a, Vec4r* b);
Vec4r vec4r_scalar(Vec4r* a, real b);
real vec4r_square_mag(Vec4r* a);
real vec4r_mag(Vec4r* a);
Vec4r vec4r_unit(Vec4r *a);
Vec4r vec4r_fill(real n);
Vec4r vec4r_zero();

#endif // VEC4R_H