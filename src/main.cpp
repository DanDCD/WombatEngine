
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <shader/shader.h>
#include <vert_data/vert_data.h>
#include <stb/stb_image.h>
#include <texture/texture.h>
#include "buffer/vao/vao.h"
#include "buffer/vbo/vbo.h"
#include "buffer/ebo/ebo.h"

void checkGLError(const std::string &label)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error [" << label << "]: " << err << std::endl;
    }
}

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

int main()
{
    // set up GLFW
    glfwInit();
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

    Shader shader("shaders/test_vertex.vert", "shaders/test_fragment.frag");

    VAO vao = VAO();

    VBO rectVBO = VBO();
    rectVBO.assignVertData(GL_ARRAY_BUFFER,
                           VERT_DATA::rectangle_textured_verts,
                           sizeof(VERT_DATA::rectangle_textured_verts),
                           GL_STATIC_DRAW);

    EBO rectEBO = EBO();
    rectEBO.assignIndiceData(VERT_DATA::rectangle_indices,
                             sizeof(VERT_DATA::rectangle_indices),
                             GL_STATIC_DRAW);

    VertexBufferLayout layout = VertexBufferLayout();
    layout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE);
    layout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE);
    layout.addAttribute(GL_FLOAT, 2, 2 * sizeof(float), GL_FALSE);

    vao.addVBO(std::move(rectVBO), layout);
    vao.addEBO(std::move(rectEBO));

    Texture texture_1(GL_TEXTURE_2D,
                      {TextureParam(GL_TEXTURE_WRAP_S, GL_REPEAT),
                       TextureParam(GL_TEXTURE_WRAP_T, GL_REPEAT),
                       TextureParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
                       TextureParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR)},
                      "textures/container.jpg",
                      GL_TEXTURE0);

    Texture texture_2(GL_TEXTURE_2D,
                      {TextureParam(GL_TEXTURE_WRAP_S, GL_REPEAT),
                       TextureParam(GL_TEXTURE_WRAP_T, GL_REPEAT),
                       TextureParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
                       TextureParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR)},
                      "textures/awesomeface.png",
                      GL_TEXTURE1);

    // we only have to set uniforms once!
    shader.use();
    shader.setUniform("texture_1", 0); // texture1 is in GL_TEXTURE0
    shader.setUniform("texture_2", 1); // texture2 is in GL_TEXTURE1

    // keep doing this loop until user wants to close
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        texture_1.bind();
        texture_2.bind();

        vao.bind();
        checkGLError("VAO bound in main loop");

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        processInput(window);

        glfwSwapBuffers(window); // swap the buffer we have been drawing to into the front
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}