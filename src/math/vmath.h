#ifndef VMATH_H
#define VMATH_H

// typedef struct vec2f {
//     float x, y;
// } vec2f;

// TODO(mitch):
// 
//  general functions:
//   zero, up, down, left, right, fill
//   add, sub, mul, div, cross, dot, mag, unit (scal and vec)
//   *projection
// 
//  special functions
//   add, sub, mul
//   scale, transform, rotate (sin cos, ignore quat for now)
//   --- remember that order matters for MVP matricies (model view projection)
//   --- the gpu will handle computing the MVP.
//   perspective, lookat
// 
//  crazy sauce
//   free look camera... you'll need to hook into SDL callbacks for mouse pos.

// NOTE(justin)
// 
// Perspective Matrix
// 
//  https://webgl.brown37.net/09_projections/04_projections_perspective_math.html
//  Look at eq8
// 
// Could be intresting to look at other projection matricies just for funsies
//  https://en.wikipedia.org/wiki/3D_projection
// 
// Maybe have an option on our eventual camera to switch between perspective
// matricies...
// 
// 

#include "type.h"
#include "vec3r.h"
#include "vec4r.h"
#include "mat4x4r.h"
#include "quat.h"

#endif // VMATH_H