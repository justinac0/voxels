#ifndef VOXEL_H
#define VOXEL_H

#include "../math/vmath.h"

typedef union voxel {
    real data[10];
    struct {
        vec3r position;
        vec3r normal;
        vec4r color;
    };
} voxel;

#endif // VOXEL_H