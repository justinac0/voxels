// https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
#ifndef QUAT_H
#define QUAT_H

#include <math.h>
#include "type.h"
#include "vec3r.h"
#include "vec4r.h"

typedef struct Quat {
    real w, x, y, z;
} Quat;

typedef struct AxisAngle {
    real x, y, z;
    real theta;
} AxisAngle;

Quat quat_from_axis_angle(real theta, Vec3r* v);
real quat_mag(Quat* quat);
Quat quat_unit(Quat* quat);
Quat quat_identity();
Quat quat_conj(Quat* quat);
real quat_dot(Quat* a, Quat* b);
Quat quat_mul(Quat* a, Quat* b);
AxisAngle axis_angle_from_quat(Quat *quat);

#endif // QUAT_H