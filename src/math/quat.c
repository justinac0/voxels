#include "quat.h"

Quat quat_from_axis_angle(real theta, Vec3r* v) {
    Quat quat;
    quat.w = cos(theta/2);
    quat.x = v->x * sin(theta/2);
    quat.y = v->y * sin(theta/2);
    quat.z = v->z * sin(theta/2);

    return quat;
}

real quat_mag(Quat* quat) {
    real mag = sqrt(quat->w*quat->w* + quat->x*quat->x + quat->y*quat->y + quat->z*quat->z);
}

Quat quat_identity() {
     return (Quat){
        .w = 1,
        .x = 0,
        .y = 0,
        .z = 0
    };
}

Quat quat_unit(Quat* quat) {
    real mag = quat_mag(quat);
    Quat q = *quat;
    q.w = q.w / mag;
    q.x = q.x / mag;
    q.y = q.y / mag;
    q.z = q.z / mag;

    return q;
}

Quat quat_conj(Quat* quat) {
    Quat q = *quat;
    q.w = -q.w;
    q.x = -q.x;
    q.y = -q.y;
    q.z = -q.z;

    return q;
}

real quat_dot(Quat* a, Quat* b) {
    return a->w*b->w + a->x*b->x + a->y*b->y + a->z*b->z;
}

Quat quat_mul(Quat* a, Quat* b) {
    Quat q;
    q.w = a->w*b->w - a->x*b->x - a->y*b->y - a->z*b->z;
    q.x = a->w*b->x + a->x*b->w + a->y*b->z - a->z*b->y;
    q.y = a->w*b->y - a->x*b->z + a->y*b->w + a->z*b->x;
    q.z = a->w*b->z + a->x*b->y + a->y*b->x + a->z*b->w;

    return q;
}

AxisAngle axis_angle_from_quat(Quat *quat) {
    AxisAngle axis;
    axis.theta = 2*acos(quat->w);
    axis.x = quat->x / sin(axis.theta/2);
    axis.y = quat->y / sin(axis.theta/2);
    axis.z = quat->z / sin(axis.theta/2);

    return axis;
}