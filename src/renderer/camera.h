#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>

#include "../input/keyboard.h"

#include "../math/vmath.h"

typedef enum {
    CAM_FREE_MOVE,
} CameraMode;

typedef struct {
    Vec3r       position;
    Quat        orientation;
    Mat4x4r     view;
    CameraMode  mode;
} Camera;

void camera_init(Camera *camera, Vec3r position);
void camera_move(Camera *camera, real mouseX, real mouseY);

#endif // CAMERA_H