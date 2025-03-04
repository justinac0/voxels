#version 330 core

in vec3 fragColor;
out vec4 FragColor;

// #define CUBE_VERTS {\
//     /*front*/\
//     -0.5,  0.5, 0.5,\
//      0.5,  0.5, 0.5,\
//     -0.5, -0.5, 0.5,\
//      0.5, -0.5, 0.5,\
// }
// /*back*/\
// /*
// -0.5,  0.5, -0.5,\
// 0.5,  0.5, -0.5,\
// -0.5, -0.5, -0.5,\
// 0.5, -0.5, -0.5,\
// \
// */

// #define CUBE_IDXS {\
//     0,3,2,\
//     0,1,3,\
// }

//
void main() {
    FragColor = vec4(fragColor/255.0f, 1.0f);
}