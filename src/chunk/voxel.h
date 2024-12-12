#ifndef VOXEL_H
#define VOXEL_H

typedef enum {
    VOXEL_TYPE_NONE  = 0,
    VOXEL_TYPE_BLACK = 1,
    VOXEL_TYPE_WHITE,
} VoxelType;

typedef struct {
    uint16_t type;
} Voxel;

// Vec3r voxel_get_colour(VoxelType type);

#endif // VOXEL_H