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

Mat4x4r mat4x4r_translation(Vec3r v) {
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

    real xx = x * x;
    real yy = y * y;
    real zz = z * z;
    real xy = x * y;
    real xz = x * z;
    real yz = y * z;
    real wx = w * x;
    real wy = w * y;
    real wz = w * z;

    Mat4x4r r = mat4x4r_identity();
    r.m00 = 1 - 2 * (yy + zz);
    r.m01 = 2 * (xy - wz);
    r.m02 = 2 * (xz + wy);

    r.m10 = 2 * (xy + wz);
    r.m11 = 1 - 2 * (xx + zz);
    r.m12 = 2 * (yz - wx);

    r.m20 = 2 * (xz - wy);
    r.m21 = 2 * (yz + wx);
    r.m22 = 1 - 2 * (xx + yy);

    return r;
}

// https://stackoverflow.com/questions/58615238/opengl-perspective-projection-how-to-define-left-and-right
Mat4x4r mat4x4r_perspective(real left, real right, real top, real bottom, real znear, real zfar, real fov, real aspect) {
    Mat4x4r p = mat4x4r_identity();

    real range = tan(fov / 2.0);

    p.m00 = 1/(range*aspect);
    p.m11 = 1/range;

    p.m22 = -(zfar + znear) / (zfar - znear);
    p.m23 = -1;
    p.m32 = -((2.0 * zfar * znear) / (zfar - znear));

    return p;
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