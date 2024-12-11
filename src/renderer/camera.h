#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>

#include "../math/vec3r.h"
#include "../math/mat4x4r.h"

typedef enum CameraMode {
    CAM_FREE_MOVE,  // complete free moving around some scene
} CameraMode;

typedef struct Camera {
    Mat4x4r     view; // this is the matrix that gets sent to
                      // the shader, apply look_at to this matrix
    Mat4x4r     perspective;
    CameraMode  mode; // NOTE: just implment free move for now
} Camera;

void camera_init(Camera *camera, Vec3r position);
void camera_move(Camera *camera, real mouseX, real mouseY, size_t screenWidth, size_t screenHeight, real fov);

#endif // CAMERA_H