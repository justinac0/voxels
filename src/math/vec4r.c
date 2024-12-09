#include "vec4r.h"

vec4r vec4r_add(vec4r* a, vec4r* b) {
    vec4r c;

    for (size_t i = 0; i < 4; i++) {
        c.v[i] = a->v[i] + b->v[i];
    }

    return c;
}

vec4r vec4r_sub(vec4r* a, vec4r* b) {
    vec4r c;

    for (size_t i = 0; i < 4; i++) {
        c.v[i] = a->v[i] - b->v[i];
    }

    return c;
}

vec4r vec4r_mul(vec4r* a, vec4r* b) {
    vec4r c;

    for (size_t i = 0; i < 4; i++) {
        c.v[i] = a->v[i] * b->v[i];
    }

    return c;
}

real vec4r_dot(vec4r* a, vec4r* b) {
    real sum = 0;

    for (size_t i = 0; i < 4; i++) {
        sum += a->v[i] * b->v[i];
    }

    return sum;
}

vec4r vec4r_scalar(vec4r* a, real b) {
    vec4r c;

    for (size_t i = 0; i < 4; i++) {
        c.v[i] = a->v[i] * b;
    }

    return c;
}

real vec4r_square_mag(vec4r* a) {
    real sum = 0;
    
    for (size_t i = 0; i < 4; i++) {
        sum += a->v[i] * a->v[i];
    }

    return sum;
}

real vec4r_mag(vec4r* a) {
    return sqrtr(vec4r_square_mag(a));
}

vec4r vec4r_unit(vec4r *a) {
    return vec4r_scalar(a, vec4r_mag(a));
}

vec4r vec4r_fill(real n) {
    vec4r v = { n };

    return v;
}

vec4r vec4r_zero() {
    return vec4r_fill(0);
}