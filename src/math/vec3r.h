#ifndef VEC3R_H
#define VEC3R_H

#include "type.h"

typedef union {
    real v[3];
    struct {
        real x, y, z;
    };
    struct {
        real r, g, b;
    };
} Vec3r;

Vec3r vec3r(real x, real y, real z);
Vec3r vec3r_add(Vec3r* a, Vec3r* b);
Vec3r vec3r_sub(Vec3r* a, Vec3r* b);
Vec3r vec3r_mul(Vec3r* a, Vec3r* b);
real vec3r_dot(Vec3r* a, Vec3r* b);
Vec3r vec3r_cross(Vec3r* a, Vec3r* b);
Vec3r vec3r_scalar(Vec3r* a, real b);
Vec3r vec3r_negate(Vec3r* a);
real vec3r_square_mag(Vec3r* a);
real vec3r_mag(Vec3r* a);
Vec3r vec3r_unit(Vec3r *a);
Vec3r vec3r_up();
Vec3r vec3r_down();
Vec3r vec3r_left();
Vec3r vec3r_right();
Vec3r vec3r_forward();
Vec3r vec3r_backward();
Vec3r vec3r_fill(real n);
Vec3r vec3r_zero();
// angle between vectors
// projection

#endif // VEC3R_H