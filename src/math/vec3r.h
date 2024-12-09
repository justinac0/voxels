#ifndef VEC3R_H
#define VEC3R_H

#include "type.h"

typedef union vec3r {
    real v[3];
    struct {
        real x, y, z;
    };
    struct {
        real r, g, b;
    };
} vec3r;

vec3r vec3r_add(vec3r* a, vec3r* b);
vec3r vec3r_sub(vec3r* a, vec3r* b);
vec3r vec3r_mul(vec3r* a, vec3r* b);
real vec3r_dot(vec3r* a, vec3r* b);
vec3r vec3r_cross(vec3r* a, vec3r* b);
vec3r vec3r_scalar(vec3r* a, real b);
real vec3r_square_mag(vec3r* a);
real vec3r_mag(vec3r* a);
vec3r vec3r_unit(vec3r *a);
vec3r vec3r_up(real n);
vec3r vec3r_down(real n);
vec3r vec3r_left(real n);
vec3r vec3r_right(real n);
vec3r vec3r_forward(real n);
vec3r vec3r_backward(real n);
vec3r vec3r_fill(real n);
vec3r vec3r_zero();

#endif // VEC3R_H