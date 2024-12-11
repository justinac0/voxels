#include "camera.h"

void camera_init(Camera *camera, Vec3r position) {
    camera->view = mat4x4r_lookat(vec3r_right(), vec3r_up(), vec3r_forward(), position);
}

void camera_move(Camera *camera, real mouseX, real mouseY, size_t screenWidth, size_t screenHeight, real fov) {
    real sensitivity = 0.05f;

    if (mouseX == 0 && mouseY == 0) return;


    Vec3r up = vec3r_up();
    Vec3r right = vec3r_right();
    // Vec3r up = vec3r_normalize(vec3r_sub(camera->target, camera->position));
    // Vec3r right = vec3r_normalize(vec3r_cross(forward, camera->up));
    Mat4x4r rotationY = mat4x4r_rotate(&camera->view, &right, -mouseY * sensitivity);
    Mat4x4r rotationX = mat4x4r_rotate(&camera->view, &up, -mouseX * sensitivity);

    camera->view = mat4x4r_mul(&camera->view, &rotationX);
    camera->view = mat4x4r_mul(&camera->view, &rotationY);

    printf("[MOUSE] mx: %02f, my: %02f\n", mouseX, mouseY);
}