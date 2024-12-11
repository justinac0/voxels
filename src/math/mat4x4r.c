#include "mat4x4r.h"

Mat4x4r mat4x4r_identity() {
    Mat4x4r m = { 0 };
    m.m00 = m.m11 = m.m22 = m.m33 = 1;

    return m;
}

Mat4x4r mat4x4r_add(Mat4x4r* a, Mat4x4r* b) {
    Mat4x4r c;

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            c.m[y][x] = a->m[y][x] + b->m[y][x];
        }
    }

    return c;
}

Mat4x4r mat4x4r_sub(Mat4x4r* a, Mat4x4r* b) {
    Mat4x4r c;

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            c.m[y][x] = a->m[y][x] - b->m[y][x];
        }
    }

    return c;
}

Mat4x4r mat4x4r_mul(Mat4x4r* a, Mat4x4r* b) {
    Mat4x4r c;

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            c.m[y][x] = a->m[y][x] * b->m[y][x];
        }
    }

    return c;
}

Mat4x4r mat4x4r_perspective(real left, real right, real top, real bottom, real znear, real zfar, real fov, real aspect) {
    Mat4x4r p = mat4x4r_identity();

    real range = 1.0f / tan(fov / 2.0);

    p.m00 = range / aspect;

    p.m11 = range;

    p.m22 = -(zfar + znear) / (zfar - znear);
    p.m23 = -1.0;

    p.m32 = -((2.0 * zfar * znear) / (zfar - znear));

    return p;
}

Mat4x4r mat4x4r_lookat_row(Vec3r right, Vec3r up, Vec3r forward, Vec3r eye) {
    Mat4x4r m = mat4x4r_identity();
    m.m00 = right.x;
    m.m01 = right.y;
    m.m02 = right.z;
    m.m03 = -vec3r_dot(&right, &eye);

    m.m10 = up.x;
    m.m11 = up.y;
    m.m12 = up.z;
    m.m13 = -vec3r_dot(&up, &eye);

    m.m20 = -forward.x;
    m.m21 = -forward.y;
    m.m22 = -forward.z;
    m.m23 = vec3r_dot(&forward, &eye);

    m.m33 = 1;

    return m;
}

Mat4x4r mat4x4r_lookat_col(Vec3r right, Vec3r up, Vec3r forward, Vec3r eye) {
    Mat4x4r m = mat4x4r_identity();
    m.m00 = right.x;
    m.m10 = right.y;
    m.m20 = right.z;
    m.m30 = -vec3r_dot(&right, &eye);

    m.m01 = up.x;
    m.m11 = up.y;
    m.m21 = up.z;
    m.m31 = -vec3r_dot(&up, &eye);

    m.m02 = -forward.x;
    m.m12 = -forward.y;
    m.m22 = -forward.z;
    m.m32 = vec3r_dot(&forward, &eye);

    m.m33 = 1;

    return m;
}