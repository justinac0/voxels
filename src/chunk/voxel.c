#include "voxel.h"

Vec4r voxel_color(VoxelType type) {
    switch (type) {
        case VOXEL_TYPE_WHITE: return vec4r(255, 255, 255, 255);
        case VOXEL_TYPE_BLACK: return vec4r(0, 0, 0, 255);
        case VOXEL_TYPE_BLUE: return vec4r(0, 0, 170, 255);
        case VOXEL_TYPE_GREEN: return vec4r(0, 170, 0, 255);
        case VOXEL_TYPE_CYAN: return vec4r(0, 170, 170, 255);
        case VOXEL_TYPE_RED: return vec4r(170, 0, 0, 255);
        case VOXEL_TYPE_MAGENTA: return vec4r(170, 0, 170, 255);
        case VOXEL_TYPE_BROWN: return vec4r(170, 85, 0, 255);
        case VOXEL_TYPE_GRAY: return vec4r(170, 170, 170, 255);
        case VOXEL_TYPE_DARK_GRAY: return vec4r(85, 85, 85, 255);
        case VOXEL_TYPE_LIGHT_BLUE: return vec4r(85, 85, 255, 255);
        case VOXEL_TYPE_LIGHT_GREEN: return vec4r(85, 255, 85, 255);
        case VOXEL_TYPE_LIGHT_CYAN: return vec4r(85, 255, 255, 255);
        case VOXEL_TYPE_LIGHT_RED: return vec4r(255, 85, 85, 255);
        case VOXEL_TYPE_LIGHT_MAGENTA: return vec4r(255, 85, 255, 255);
        case VOXEL_TYPE_YELLOW: return vec4r(255, 255, 85, 255);
    default:
        return vec4r(0, 0, 0, 0);
    }
}
