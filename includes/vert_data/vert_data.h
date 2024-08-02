#pragma once

namespace VERT_DATA
{

    float vertices[] = {
        // x, y, z,
        // positions
        // Front face
        -0.5f, -0.5f, 0.5f, // 0
        0.5f, -0.5f, 0.5f,  // 1
        0.5f, 0.5f, 0.5f,   // 2
        -0.5f, 0.5f, 0.5f,  // 3
        // Back face
        -0.5f, -0.5f, -0.5f, // 4
        0.5f, -0.5f, -0.5f,  // 5
        0.5f, 0.5f, -0.5f,   // 6
        -0.5f, 0.5f, -0.5f,  // 7
        // Left face
        -0.5f, -0.5f, -0.5f, // 8
        -0.5f, -0.5f, 0.5f,  // 9
        -0.5f, 0.5f, 0.5f,   // 10
        -0.5f, 0.5f, -0.5f,  // 11
        // Right face
        0.5f, -0.5f, -0.5f, // 12
        0.5f, -0.5f, 0.5f,  // 13
        0.5f, 0.5f, 0.5f,   // 14
        0.5f, 0.5f, -0.5f,  // 15
        // Top face
        -0.5f, 0.5f, -0.5f, // 16
        0.5f, 0.5f, -0.5f,  // 17
        0.5f, 0.5f, 0.5f,   // 18
        -0.5f, 0.5f, 0.5f,  // 19
        // Bottom face
        -0.5f, -0.5f, -0.5f, // 20
        0.5f, -0.5f, -0.5f,  // 21
        0.5f, -0.5f, 0.5f,   // 22
        -0.5f, -0.5f, 0.5f,  // 23
    };
    float tex_coords[] = {
        // tex_x, tex_y
        // Front face
        0.0f, 0.0f, // 0
        1.0f, 0.0f, // 1
        1.0f, 1.0f, // 2
        0.0f, 1.0f, // 3
        // Back face
        0.0f, 0.0f, // 4
        1.0f, 0.0f, // 5
        1.0f, 1.0f, // 6
        0.0f, 1.0f, // 7
        // Left face
        0.0f, 0.0f, // 8
        1.0f, 0.0f, // 9
        1.0f, 1.0f, // 10
        0.0f, 1.0f, // 11
        // Right face
        0.0f, 0.0f, // 12
        1.0f, 0.0f, // 13
        1.0f, 1.0f, // 14
        0.0f, 1.0f, // 15
        // Top face
        0.0f, 0.0f, // 16
        1.0f, 0.0f, // 17
        1.0f, 1.0f, // 18
        0.0f, 1.0f, // 19
        // Bottom face
        0.0f, 0.0f, // 20
        1.0f, 0.0f, // 21
        1.0f, 1.0f, // 22
        0.0f, 1.0f  // 23
    };

    float normals[] = {
        // Front face
        0.0f, 0.0f, 1.0f, // Normal for vertex 0
        0.0f, 0.0f, 1.0f, // Normal for vertex 1
        0.0f, 0.0f, 1.0f, // Normal for vertex 2
        0.0f, 0.0f, 1.0f, // Normal for vertex 3

        // Back face
        0.0f, 0.0f, -1.0f, // Normal for vertex 4
        0.0f, 0.0f, -1.0f, // Normal for vertex 5
        0.0f, 0.0f, -1.0f, // Normal for vertex 6
        0.0f, 0.0f, -1.0f, // Normal for vertex 7

        // Left face
        -1.0f, 0.0f, 0.0f, // Normal for vertex 8
        -1.0f, 0.0f, 0.0f, // Normal for vertex 9
        -1.0f, 0.0f, 0.0f, // Normal for vertex 10
        -1.0f, 0.0f, 0.0f, // Normal for vertex 11

        // Right face
        1.0f, 0.0f, 0.0f, // Normal for vertex 12
        1.0f, 0.0f, 0.0f, // Normal for vertex 13
        1.0f, 0.0f, 0.0f, // Normal for vertex 14
        1.0f, 0.0f, 0.0f, // Normal for vertex 15

        // Top face
        0.0f, 1.0f, 0.0f, // Normal for vertex 16
        0.0f, 1.0f, 0.0f, // Normal for vertex 17
        0.0f, 1.0f, 0.0f, // Normal for vertex 18
        0.0f, 1.0f, 0.0f, // Normal for vertex 19

        // Bottom face
        0.0f, -1.0f, 0.0f, // Normal for vertex 20
        0.0f, -1.0f, 0.0f, // Normal for vertex 21
        0.0f, -1.0f, 0.0f, // Normal for vertex 22
        0.0f, -1.0f, 0.0f, // Normal for vertex 23
    };

    float verts_with_normals[] = {
        // x, y, z, nx, ny, nz
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 0
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // 1
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // 2
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // 3
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 4
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // 5
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   // 6
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // 7
        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 8
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  // 9
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   // 10
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // 11
        // Right face
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 12
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // 13
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // 14
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // 15
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 16
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 17
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // 18
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // 19
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // 20
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  // 21
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,   // 22
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,  // 23
    };

    float verts_with_normals_and_texcoords[] = {
        // x, y, z,        nx, ny, nz,   tx, ty
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // 1
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // 2
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 3
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 4
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // 5
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // 6
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // 7
        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 8
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 9
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 10
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 11
        // Right face
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 12
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 13
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 14
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 15
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 16
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 17
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // 18
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 19
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 20
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // 21
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // 22
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // 23
    };

    unsigned int indices[] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20};

}
