# pragma once


namespace VERT_DATA{

    float colored_triangle_verts[] = {
        // positions         colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f
    };

    float triangle_verts[] = {
        // positions
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    

    float rectangle_verts[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    unsigned int rectangle_indices[] = {
        // clockwise
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
}

