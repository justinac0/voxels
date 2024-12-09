#ifndef VEC4R_H
#define VEC4R_H

#include "type.h"

typedef union vec4r {
    real v[4];
    struct {
        real x, y, z, w;
    };
    struct {
        real r, g, b, a;
    };
} vec4r;

vec4r vec4r_add(vec4r* a, vec4r* b);
vec4r vec4r_sub(vec4r* a, vec4r* b);
vec4r vec4r_mul(vec4r* a, vec4r* b);
real vec4r_dot(vec4r* a, vec4r* b);
vec4r vec4r_scalar(vec4r* a, real b);
real vec4r_square_mag(vec4r* a);
real vec4r_mag(vec4r* a);
vec4r vec4r_unit(vec4r *a);
vec4r vec4r_fill(real n);
vec4r vec4r_zero();

#endif // VEC4R_H