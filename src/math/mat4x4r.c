#include "mat4x4r.h"

mat4x4r mat4x4r_identity() {
    mat4x4r m = { 0 };
    m.m00 = m.m11 = m.m22 = m.m33 = 1;

    return m;
}

mat4x4r mat4x4r_add(mat4x4r* a, mat4x4r* b) {
    mat4x4r c;

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            c.m[y][x] = a->m[y][x] + b->m[y][x];
        }
    }

    return c;
}

mat4x4r mat4x4r_sub(mat4x4r* a, mat4x4r* b) {
    mat4x4r c;

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            c.m[y][x] = a->m[y][x] - b->m[y][x];
        }
    }

    return c;
}

mat4x4r mat4x4r_mul(mat4x4r* a, mat4x4r* b) {
    mat4x4r c;

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            c.m[y][x] = a->m[y][x] * b->m[y][x];
        }
    }

    return c;
}

// mat4x4r_perspective: left, right, top, bottom, znear, zfar, fov
mat4x4r mat4x4r_perspective(real l, real r, real t, real b, real near, real far, real fov, real aspect) {
    mat4x4r p = mat4x4r_identity();

    real range = 1.0f / tan(fov / 2.0);

    p.m00 = range / aspect;

    p.m11 = range;

    p.m22 = -(far + near) / (far - near);
    p.m23 = -1.0;

    p.m32 = -((2.0 * far * near) / (far - near));


    return p;
}