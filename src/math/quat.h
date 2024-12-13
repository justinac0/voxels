// https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
#ifndef QUAT_H
#define QUAT_H

#include <math.h>
#include "type.h"
#include "vec3r.h"
#include "vec4r.h"

#include <stdio.h>

typedef struct {
    real w, x, y, z;
} Quat;

typedef struct {
    Vec3r position;
    real theta;
} AxisAngle;

AxisAngle axis_angle(real theta, Vec3r position);

Quat quat_from_axis_angle(AxisAngle axis);
real quat_mag(Quat* quat);
Quat quat_unit(Quat* quat);
Quat quat_identity();
Vec3r quat_rotate_vec3r(Quat* quat, Vec3r* vec);
Quat quat_conj(Quat* quat);
real quat_dot(Quat* a, Quat* b);
Quat quat_mul(Quat* a, Quat* b);
AxisAngle axis_angle_from_quat(Quat *quat);

#endif // QUAT_H