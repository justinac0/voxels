#include "camera.h"

void camera_init(Camera *camera, Vec3r position) {
    camera->position = (Vec3r){
        .x = position.x,
        .y = position.y,
        .z = position.z
    };

    camera->orientation = (Quat){0};

    camera->view = mat4x4r_identity();
    camera->view = mat4x4r_translate(camera->position);
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

    Mat4x4r rotation = mat4x4r_identity();
    Mat4x4r translation = mat4x4r_translate((Vec3r){x,y,z});

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
    rotation = mat4x4r_rotation(camera->orientation);

    camera->view = mat4x4r_mul(&camera->view, &rotation);
    camera->view = mat4x4r_mul(&camera->view, &translation);
}