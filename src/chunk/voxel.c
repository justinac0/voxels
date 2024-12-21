#include "voxel.h"

Vec3r voxel_color(VoxelType type) {
    switch (type) {
        case VOXEL_TYPE_WHITE:          return vec3r(255, 255, 255);
        case VOXEL_TYPE_BLACK:          return vec3r(0, 0, 0);
        case VOXEL_TYPE_BLUE:           return vec3r(0, 0, 170);
        case VOXEL_TYPE_GREEN:          return vec3r(0, 170, 0);
        case VOXEL_TYPE_CYAN:           return vec3r(0, 170, 170);
        case VOXEL_TYPE_RED:            return vec3r(170, 0, 0);
        case VOXEL_TYPE_MAGENTA:        return vec3r(170, 0, 170);
        case VOXEL_TYPE_BROWN:          return vec3r(170, 85, 0);
        case VOXEL_TYPE_GRAY:           return vec3r(170, 170, 170);
        case VOXEL_TYPE_DARK_GRAY:      return vec3r(85, 85, 85);
        case VOXEL_TYPE_LIGHT_BLUE:     return vec3r(85, 85, 255);
        case VOXEL_TYPE_LIGHT_GREEN:    return vec3r(85, 255, 85);
        case VOXEL_TYPE_LIGHT_CYAN:     return vec3r(85, 255, 255);
        case VOXEL_TYPE_LIGHT_RED:      return vec3r(255, 85, 85);
        case VOXEL_TYPE_LIGHT_MAGENTA:  return vec3r(255, 85, 255);
        case VOXEL_TYPE_YELLOW:         return vec3r(255, 255, 85);
    default:
        return vec3r(0, 0, 0);
    }
}
