#include "vec3r.h"

vec3r vec3r_add(vec3r* a, vec3r* b) {
    vec3r c;

    for (size_t i = 0; i < 3; i++) {
        c.v[i] = a->v[i] + b->v[i];
    }

    return c;
}

vec3r vec3r_sub(vec3r* a, vec3r* b) {
    vec3r c;

    for (size_t i = 0; i < 3; i++) {
        c.v[i] = a->v[i] - b->v[i];
    }

    return c;
}

vec3r vec3r_mul(vec3r* a, vec3r* b) {
    vec3r c;

    for (size_t i = 0; i < 3; i++) {
        c.v[i] = a->v[i] * b->v[i];
    }

    return c;
}

real vec3r_dot(vec3r* a, vec3r* b) {
    real sum = 0;

    for (size_t i = 0; i < 3; i++) {
        sum += a->v[i] * b->v[i];
    }

    return sum;
}

vec3r vec3r_cross(vec3r* a, vec3r* b) {
    vec3r c;

    c.x = a->y * b->z - a->z * b->y;
    c.y = a->z * b->x - a->x * b->z;
    c.z = a->x * b->y - a->y * b->x;

    return c;
}

vec3r vec3r_scalar(vec3r* a, real b) {
    vec3r c;

    for (size_t i = 0; i < 3; i++) {
        c.v[i] = a->v[i] * b;
    }

    return c;
}

real vec3r_square_mag(vec3r* a) {
    real sum = 0;
    
    for (size_t i = 0; i < 3; i++) {
        sum += a->v[i] * a->v[i];
    }

    return sum;
}

real vec3r_mag(vec3r* a) {
    return sqrtr(vec3r_square_mag(a));
}

vec3r vec3r_unit(vec3r *a) {
    return vec3r_scalar(a, vec3r_mag(a));
}

vec3r vec3r_up(real n) {
    vec3r v = {
        0, 1, 0,
    };

    return v;
}

vec3r vec3r_down(real n) {
    vec3r v = {
        0, -1, 0,
    };

    return v;
}

vec3r vec3r_left(real n) {
    vec3r v = {
        -1, 0, 0,
    };

    return v;
}

vec3r vec3r_right(real n) {
    vec3r v = {
        1, 0, 0,
    };

    return v;
}

vec3r vec3r_forward(real n) {
    vec3r v = {
        0, 0, 1,
    };

    return v;
}

vec3r vec3r_backward(real n) {
    vec3r v = {
        0, 0, -1,
    };

    return v;
}

vec3r vec3r_fill(real n) {
    vec3r v = { n };

    return v;
}

vec3r vec3r_zero() {
    return vec3r_fill(0);
}