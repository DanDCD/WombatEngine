
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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    glEnable(GL_DEPTH_TEST); // enable depth TODO: Update comment

    // tell OpenGL the size of the rendering Window
    glViewport(0, 0, 800, 600);
    // assign our resizing function as the resizing window callback for our window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader("shaders/test_vertex.vert", "shaders/test_fragment.frag");

    VAO vao = VAO();

    VBO rectVBO = VBO();
    rectVBO.assignVertData(GL_ARRAY_BUFFER,
                           VERT_DATA::cube,
                           sizeof(VERT_DATA::cube),
                           GL_STATIC_DRAW);

    // TODO: use EBO again for cube
    // EBO rectEBO = EBO();
    // rectEBO.assignIndiceData(VERT_DATA::rectangle_indices,
    //                          sizeof(VERT_DATA::rectangle_indices),
    //                          GL_STATIC_DRAW);

    VertexBufferLayout layout = VertexBufferLayout();
    layout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE); // vertex local position
    layout.addAttribute(GL_FLOAT, 2, 2 * sizeof(float), GL_FALSE); // texture position

    vao.addVBO(std::move(rectVBO), layout);
    // vao.addEBO(std::move(rectEBO));

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

    // box positions
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    // we only have to set uniforms once!
    shader.use();
    shader.setUniform("texture_1", 0); // texture1 is in GL_TEXTURE0
    shader.setUniform("texture_2", 1); // texture2 is in GL_TEXTURE1

    // keep doing this loop until user wants to close
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // transformations
        // view matrix
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // projection matrix
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shader.use();
        // we no longer set mode here
        shader.setUniform("view", 1, false, view); // set the view matrix
        shader.setUniform("projection", 1, false, projection); // set the projection matrix

        texture_1.bind();
        texture_2.bind();

        vao.bind();
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 1.0f + 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setUniform("model", 1, false, model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); not using EBO

        processInput(window);

        glfwSwapBuffers(window); // swap the buffer we have been drawing to into the front
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}