#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>

#include "../input/keyboard.h"
#include "../math/quat.h"

#include "../math/vec3r.h"
#include "../math/mat4x4r.h"

typedef enum {
    CAM_FREE_MOVE,
} CameraMode;

typedef struct {
    Vec3r       position;
    Quat        orientation;
    Mat4x4r     view;
    Mat4x4r     rotation;
    Mat4x4r     translation;
    CameraMode  mode;
} Camera;

void camera_init(Camera *camera, Vec3r position);
void camera_move(Camera *camera, real mouseX, real mouseY, size_t screenWidth, size_t screenHeight, real fov);

#endif // CAMERA_H