#ifndef VOXEL_H
#define VOXEL_H

#include "../math/vmath.h"

typedef union Voxel {
    real data[10];
    struct {
        Vec3r position;
        Vec3r normal;
        Vec4r color;
    };
} Voxel;

#endif // VOXEL_H