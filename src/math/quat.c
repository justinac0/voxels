#include "quat.h"

Quat quat_from_axis_angle(real theta, real x, real y, real z) {
    Quat quat;
    quat.q0 = cos(theta/2);
    quat.q1 = x * sin(theta/2);
    quat.q2 = y * sin(theta/2);
    quat.q3 = z * sin(theta/2);

    return quat;
}

AxisAngle axis_angle_from_quat(Quat *quat) {
    AxisAngle aa;
    aa.theta = 2*acos(quat->q0);
    aa.x = quat->q1/sin(aa.theta/2);
    aa.y = quat->q2/sin(aa.theta/2);
    aa.z = quat->q3/sin(aa.theta/2);

    return aa;
}