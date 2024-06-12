
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <shader/shader.h>
#include <vert_data/vert_data.h>

// A callback function to be called whenever the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // tell opengl the new window size (if changed)
}

// function to process input events from user
void processInput(GLFWwindow *window)
{
    // if user presses escpape, we tell GLFW we want to close the given window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// set up our triangle
void setUpTriangle()
{
    // this will be assigned the id of the vertex buffer object
    unsigned int vbo_id;
    // create one vertex buffer object and store its id in vbo_id for future reference
    glGenBuffers(1, &vbo_id);
    // we bind the buffer objeect to the array buffer - meaning when we use the array buffer we will be using the object
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    // we now store the triangle verts in the vbo bound to the array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERT_DATA::colored_triangle_verts), VERT_DATA::colored_triangle_verts, GL_STATIC_DRAW);
}

// set up our rectangle - conversely, we use EBO and index drawing to save memory!
void setUpRectangle()
{
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERT_DATA::rectangle_verts), VERT_DATA::rectangle_verts, GL_STATIC_DRAW);

    unsigned int ebo_id;
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VERT_DATA::rectangle_indices), VERT_DATA::rectangle_indices, GL_STATIC_DRAW);
}

unsigned int setUpVAO()
{
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    return vao_id;
}

int main()
{
    // set up GLFW
    glfwInit(); // Initialize the GLFW library
    // Set the major and minor version of OpenGL to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set the OpenGL profile to use
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "ThreeDimSim", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialise GLAD
    bool loaded_GLAD = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // tell GLAD to load the address of the OpenGL function pointers
    if (!loaded_GLAD)
    {
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }

    // tell OpenGL the size of the rendering Window
    glViewport(0, 0, 800, 600);
    // assign our resizing function as the resizing window callback for our window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // set up shaders
    Shader shader("shaders/test_vertex.vert", "shaders/test_fragment.frag");

    // set up vao
    unsigned int vao_id = setUpVAO();
    setUpTriangle();
    // setUpRectangle();

    // set up vertex attributes
    // for vertext attribute 0, we have 3 float values for each attrib, stride of six, we do not start with an offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // keep doing this loop until user wants to close
    while (!glfwWindowShouldClose(window))
    {
        // we clear the screen with a blue color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // we clear the color buffer

        shader.use(); // use shader

        glBindVertexArray(vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        processInput(window); // process input events

        glfwSwapBuffers(window); // swap the buffer we have been drawing to into the front
        glfwPollEvents();        // poll for events
    }

    glfwTerminate();
    return 0;
}