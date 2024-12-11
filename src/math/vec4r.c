#include "vec4r.h"

Vec4r vec4r_add(Vec4r* a, Vec4r* b) {
    Vec4r c;

    for (size_t i = 0; i < 4; i++) {
        c.v[i] = a->v[i] + b->v[i];
    }

    return c;
}

Vec4r vec4r_sub(Vec4r* a, Vec4r* b) {
    Vec4r c;

    for (size_t i = 0; i < 4; i++) {
        c.v[i] = a->v[i] - b->v[i];
    }

    return c;
}

Vec4r vec4r_mul(Vec4r* a, Vec4r* b) {
    Vec4r c;

    for (size_t i = 0; i < 4; i++) {
        c.v[i] = a->v[i] * b->v[i];
    }

    return c;
}

real vec4r_dot(Vec4r* a, Vec4r* b) {
    real sum = 0;

    for (size_t i = 0; i < 4; i++) {
        sum += a->v[i] * b->v[i];
    }

    return sum;
}

Vec4r vec4r_scalar(Vec4r* a, real b) {
    Vec4r c;

    for (size_t i = 0; i < 4; i++) {
        c.v[i] = a->v[i] * b;
    }

    return c;
}

real vec4r_square_mag(Vec4r* a) {
    real sum = 0;
    
    for (size_t i = 0; i < 4; i++) {
        sum += a->v[i] * a->v[i];
    }

    return sum;
}

real vec4r_mag(Vec4r* a) {
    return sqrtr(vec4r_square_mag(a));
}

Vec4r vec4r_unit(Vec4r *a) {
    return vec4r_scalar(a, vec4r_mag(a));
}

Vec4r vec4r_fill(real n) {
    Vec4r v = { n };

    return v;
}

Vec4r vec4r_zero() {
    return vec4r_fill(0);
}