// https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
#ifndef QUAT_H
#define QUAT_H

#include "type.h"
#include "vec3r.h"
#include "vec4r.h"

typedef struct Quat {
    real q0, q1, q2, q3;
} Quat;

typedef struct AxisAngle {
    real x, y, z;
    real theta;
} AxisAngle;

Quat quat_from_axis_angle(real theata, real x, real y, real z);
AxisAngle axis_angle_from_quat(Quat *quat);

#endif // QUAT_H