#ifndef VOXEL_H
#define VOXEL_H

#include <stdint.h>
#include "../math/vec4r.h"

typedef enum {
    VOXEL_TYPE_NONE  = 0,
    VOXEL_TYPE_WHITE = 1,
    VOXEL_TYPE_BLACK = 2,
    VOXEL_TYPE_BLUE = 3,
    VOXEL_TYPE_GREEN = 4,
    VOXEL_TYPE_CYAN = 5,
    VOXEL_TYPE_RED = 6,
    VOXEL_TYPE_MAGENTA = 7,
    VOXEL_TYPE_BROWN = 8,
    VOXEL_TYPE_GRAY = 9,
    VOXEL_TYPE_DARK_GRAY = 10,
    VOXEL_TYPE_LIGHT_BLUE = 11,
    VOXEL_TYPE_LIGHT_GREEN = 12,
    VOXEL_TYPE_LIGHT_CYAN = 13,
    VOXEL_TYPE_LIGHT_RED = 14,
    VOXEL_TYPE_LIGHT_MAGENTA = 15,
    VOXEL_TYPE_YELLOW = 16,
    VOXEL_TYPE_COUNT
} VoxelType;

typedef struct {
    VoxelType type;
} Voxel;

Vec4r voxel_color(VoxelType type);

#endif // VOXEL_H