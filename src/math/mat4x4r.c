#include "mat4x4r.h"

Mat4x4r mat4x4r_identity() {
    Mat4x4r m = { 0 };
    m.m00 = m.m11 = m.m22 = m.m33 = 1;

    return m;
}

Mat4x4r mat4x4r_transpose(Mat4x4r* m) {
    Mat4x4r t;

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            t.m[y][x] = m->m[x][y];
        }
    }

    return t;
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
            real sum = 0;
            for (size_t i = 0; i < 4; i++) {
                sum += a->m[y][i] * b->m[i][x];
            }
            c.m[y][x] = sum;
        }
    }

    return c;
}

Mat4x4r mat4x4r_mul_scalar(Mat4x4r* m, real scalar) {
    Mat4x4r c;

    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            c.m[y][x] = m->m[y][x] * scalar;
        }
    }

    return c;
}

Mat4x4r mat4x4r_scale(Mat4x4r* m, Vec3r* v) {
    Mat4x4r n = *m;

    // only iterate up to 3 because our vec3 only has 3 elements
    for (size_t i = 0; i < 3; i++) {    
        n.m[i][i] *= v->v[i];
    }

    return n;
}

Mat4x4r mat4x4r_translate(Vec3r v) {
    Mat4x4r n = mat4x4r_identity();

    for (size_t i = 0; i < 3; i++) {
        n.m[3][i] += v.v[i];
    }

    return n;
}

Mat4x4r mat4x4r_rotation(Quat q) {
    real w = q.w;
    real x = q.x;
    real y = q.y;
    real z = q.z;
    
    real ww = q.w*q.w;
    real xx = q.x*q.x;
    real yy = q.y*q.y;
    real zz = q.z*q.z;

    Mat4x4r r = mat4x4r_identity();
    r.m00 = ww+xx-yy-zz;
    r.m01 = 2*x*y-2*w*z;
    r.m02 = 2*x*z+2*w*y;

    r.m10 = 2*x*y+2*w*z;
    r.m11 = ww-xx+yy-zz;
    r.m12 = 2*y*z-2*w*x;


    r.m20 = 2*x*z-2*w*y;
    r.m21 = 2*y*z+2*w*x;
    r.m22 = ww-xx-yy+zz;

    // r.m33 = 1;

    return r;
}

Mat4x4r mat4x4r_perspective(real left, real right, real top, real bottom, real znear, real zfar, real fov, real aspect) {
    Mat4x4r p = mat4x4r_identity();

    // What happens when fov > 75? < 90? > 90?
    real range = 1.0f / tan(fov / 2.0);

    p.m00 = range / aspect;

    p.m11 = range;

    p.m22 = -(zfar + znear) / (zfar - znear);
    p.m23 = -1.0;

    p.m32 = -((2.0 * zfar * znear) / (zfar - znear));

    return p;
}

Mat4x4r mat4x4r_lookat(Vec3r right, Vec3r up, Vec3r forward, Vec3r eye) {
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

Mat4x4r mat4x4r_print(Mat4x4r* m) {
    printf("[Matrix]\n");
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            printf("%02f ", m->m[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}