#include "camera.h"

void camera_init(Camera *camera, Vec3r position) {
    camera->position = (Vec3r){
        .x = position.x,
        .y = position.y,
        .z = position.z
    };

    camera->orientation = quat_identity();
    camera->view = mat4x4r_translation(camera->position);
}

Mat4x4r construct_view_matrix(Mat4x4r* view, Mat4x4r* rotation, Mat4x4r* translation) {
    Mat4x4r v = *view;
    v = mat4x4r_mul(rotation, translation);

    return v;
}

real aYaw;
real aPitch;

void camera_move(Camera *camera, real mouseX, real mouseY) {
    real moveSpeed = 0.1;
    real sensitivity = 0.001f;

    real x = (is_key_down(SDLK_A) - is_key_down(SDLK_D)) * moveSpeed;
    real z = (is_key_down(SDLK_W) - is_key_down(SDLK_S)) * moveSpeed;
    real y = (is_key_down(SDLK_LSHIFT) - is_key_down(SDLK_SPACE)) * moveSpeed;


    real yaw = -mouseX * sensitivity;
    real pitch = -mouseY * sensitivity;

    aYaw += yaw;
    aPitch += pitch;

    Quat qx = quat_from_axis_angle(axis_angle(pitch, vec3r_right()));
    Quat qy = quat_from_axis_angle(axis_angle(yaw, vec3r_up()));

    camera->orientation = quat_mul(&qy, &camera->orientation);
    camera->orientation = quat_mul(&camera->orientation, &qx);
    camera->orientation = quat_unit(&camera->orientation);

    Vec3r localMove = vec3r(x, y, z);
    Vec3r worldMove = quat_rotate_vec3r(&camera->orientation, &localMove);
    worldMove.y = localMove.y;

    camera->position = vec3r_add(&camera->position, &worldMove);
    Mat4x4r T = mat4x4r_translation(camera->position);
    Mat4x4r R = mat4x4r_rotation(camera->orientation);

    camera->view = mat4x4r_mul(&T, &R);
}