#include "camera.h"

void camera_init(Camera *camera, Vec3r position) {
    camera->position = (Vec3r){
        .x = position.x,
        .y = position.y,
        .z = position.z
    };

    camera->orientation = quat_identity();

    camera->view = camera->rotation = camera->translation = mat4x4r_identity();
    camera->view = mat4x4r_translation(camera->position);
}

Mat4x4r construct_view_matrix(Mat4x4r* view, Vec3r* position, Mat4x4r* rotation) {
    //  Rotation
    Mat4x4r v = *view;
    v.m00 = rotation->m00;
    v.m01 = rotation->m01;
    v.m02 = rotation->m02;

    v.m10 = rotation->m10;
    v.m11 = rotation->m11;
    v.m12 = rotation->m12;

    v.m20 = rotation->m20;
    v.m21 = rotation->m21;
    v.m22 = rotation->m22;

    //  Translation
    v.m30 = position->x;
    v.m31 = position->y;
    v.m32 = position->z;
    v.m33 = 1;
}

Mat4x4r construct_view_matrix2(Mat4x4r* view, Mat4x4r* rotation, Mat4x4r* translation) {
    Mat4x4r v = *view;
    v = mat4x4r_mul(&v, rotation);
    v = mat4x4r_mul(&v, translation);
    return v;
}

void camera_move(Camera *camera, real mouseX, real mouseY, size_t screenWidth, size_t screenHeight, real fov) {
    // camera controls - TODO: move this to camera struct once all is working... also add fixed game tick updates...
    real moveSpeed = 0.01;
    real sensitivity = 0.001f;

    // handle user input
    real x = (is_key_down(SDLK_A) - is_key_down(SDLK_D)) * moveSpeed;
    real y = (is_key_down(SDLK_LSHIFT) - is_key_down(SDLK_SPACE)) * moveSpeed;
    real z = (is_key_down(SDLK_W) - is_key_down(SDLK_S)) * moveSpeed;

    camera->position = (Vec3r){
        .x = camera->position.x + x,
        .y = camera->position.y + y,
        .z = camera->position.z + z
    };

    Mat4x4r translation = mat4x4r_translation((Vec3r){x,y,z});

    // handle mouse input
    Vec3r up = vec3r_up();
    Vec3r right = vec3r_right();
    Vec3r forward = vec3r_forward();

    real yaw = -mouseX * sensitivity;
    real pitch = -mouseY * sensitivity;

    Quat qx = quat_from_axis_angle(pitch, &right);
    Quat qy = quat_from_axis_angle(yaw, &up);
    Quat q = quat_mul(&qy, &qx);

    Quat orientation = quat_identity();
    camera->orientation = quat_mul(&orientation, &q);
    camera->rotation = mat4x4r_rotation(camera->orientation);

    camera->view = construct_view_matrix2(&camera->view, &camera->rotation, &translation);
    // camera->view = construct_view_matrix(&camera->view, &camera->position, &camera->rotation);
}

    // rotate local x-axis for up, down
    // rotate global y-axis for left, right
    // Quat yaw = quat_from_axis_angle(-mouseX * sensitivity, &right);
    // Mat4x4r yawMat = mat4x4r_rotation(yaw);
    // Quat pitch = quat_from_axis_angle(-mouseY * sensitivity, &up);
    // Mat4x4r pitchMat = mat4x4r_rotation(pitch);


    // camera->orientation = quat_mul(&pitch, &camera->orientation);
    // camera->orientation = quat_mul(&camera->orientation, &yaw);

    // // Yaw happens "over" the current rotation, in global coordinates.
    // Quaternion yaw = Quaternion.Euler(0f, Input.GetAxis("Horizontal") * speed, 0f);
    // transform.rotation =  yaw * transform.rotation; // yaw on the left.

    // // Pitch happens "under" the current rotation, in local coordinates.
    // Quaternion pitch = Quaternion.Euler(-Input.GetAxis("Vertical") * speed, 0f, 0f);
    // transform.rotation = transform.rotation * pitch; // pitch on the right.

    // camera->rotation = mat4x4r_mul(&yawMat, &camera->rotation);
    // camera->rotation = mat4x4r_mul(&camera->rotation, &pitchMat);