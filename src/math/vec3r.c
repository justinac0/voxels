#include "vec3r.h"

Vec3r vec3r_add(Vec3r* a, Vec3r* b) {
    Vec3r c;

    for (size_t i = 0; i < 3; i++) {
        c.v[i] = a->v[i] + b->v[i];
    }

    return c;
}

Vec3r vec3r_sub(Vec3r* a, Vec3r* b) {
    Vec3r c;

    for (size_t i = 0; i < 3; i++) {
        c.v[i] = a->v[i] - b->v[i];
    }

    return c;
}

Vec3r vec3r_mul(Vec3r* a, Vec3r* b) {
    Vec3r c;

    for (size_t i = 0; i < 3; i++) {
        c.v[i] = a->v[i] * b->v[i];
    }

    return c;
}

real vec3r_dot(Vec3r* a, Vec3r* b) {
    real sum = 0;

    for (size_t i = 0; i < 3; i++) {
        sum += a->v[i] * b->v[i];
    }

    return sum;
}

Vec3r vec3r_cross(Vec3r* a, Vec3r* b) {
    Vec3r c;

    c.x = a->y * b->z - a->z * b->y;
    c.y = a->z * b->x - a->x * b->z;
    c.z = a->x * b->y - a->y * b->x;

    return c;
}

Vec3r vec3r_scalar(Vec3r* a, real b) {
    Vec3r c;

    for (size_t i = 0; i < 3; i++) {
        c.v[i] = a->v[i] * b;
    }

    return c;
}

real vec3r_square_mag(Vec3r* a) {
    real sum = 0;
    
    for (size_t i = 0; i < 3; i++) {
        sum += a->v[i] * a->v[i];
    }

    return sum;
}

real vec3r_mag(Vec3r* a) {
    return sqrtr(vec3r_square_mag(a));
}

Vec3r vec3r_unit(Vec3r *a) {
    return vec3r_scalar(a, vec3r_mag(a));
}

Vec3r vec3r_negate(Vec3r* a) {
    Vec3r v;
    v.x = -a->x;
    v.y = -a->y;
    v.z = -a->z;

    return v;
}

Vec3r vec3r_up() {
    Vec3r v = {
        0, 1, 0,
    };

    return v;
}

Vec3r vec3r_down() {
    Vec3r v = {
        0, -1, 0,
    };

    return v;
}

Vec3r vec3r_left() {
    Vec3r v = {
        -1, 0, 0,
    };

    return v;
}

Vec3r vec3r_right() {
    Vec3r v = {
        1, 0, 0,
    };

    return v;
}

Vec3r vec3r_forward() {
    Vec3r v = {
        0, 0, 1,
    };

    return v;
}

Vec3r vec3r_backward() {
    Vec3r v = {
        0, 0, -1,
    };

    return v;
}

Vec3r vec3r_fill(real n) {
    Vec3r v = { n };

    return v;
}

Vec3r vec3r_zero() {
    return vec3r_fill(0);
}