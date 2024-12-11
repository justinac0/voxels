#ifndef MAT4X4R_H
#define MAT4X4R_H

#include <stdio.h>

#include "type.h"
#include "vec3r.h"
#include "vec4r.h"
#include "quat.h"

typedef union Mat4x4r {
    real m[4][4];
    struct {
        real m00, m01, m02, m03;
        real m10, m11, m12, m13;
        real m20, m21, m22, m23;
        real m30, m31, m32, m33;
    };
    struct {
        Vec4r m0, m1, m2, m3;
    };
} Mat4x4r;

Mat4x4r mat4x4r_identity();
Mat4x4r mat4x4r_transpose(Mat4x4r* m);
Mat4x4r mat4x4r_add(Mat4x4r* a, Mat4x4r* b);
Mat4x4r mat4x4r_sub(Mat4x4r* a, Mat4x4r* b);
Mat4x4r mat4x4r_mul(Mat4x4r* a, Mat4x4r* b);
Mat4x4r mat4x4r_mul_scalar(Mat4x4r* m, real value);
Mat4x4r mat4x4r_scale(Mat4x4r* m, Vec3r* scale);
Mat4x4r mat4x4r_translate(Mat4x4r* m, Vec3r* translate);
Mat4x4r mat4x4r_rotate(Mat4x4r* m, Vec3r* axis, real angle);
Mat4x4r mat4x4r_rotate_x(Mat4x4r* m, real angle);
Mat4x4r mat4x4r_rotate_y(Mat4x4r* m, real angle);
Mat4x4r mat4x4r_rotate_z(Mat4x4r* m, real angle);
Mat4x4r mat4x4r_perspective(real l, real r, real t, real b, real near, real far, real fov, real aspect);
Mat4x4r mat4x4r_lookat(Vec3r right, Vec3r up, Vec3r forward, Vec3r eye);
Mat4x4r mat4x4r_print(Mat4x4r* m);

#endif // MAT4X4R_H