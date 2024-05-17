
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

float triangle_verts[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts), triangle_verts, GL_STATIC_DRAW);
}

// vertex shader
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// set up the vertex shader
unsigned int setUpVertexShader()
{
    unsigned int vertexShader_id;
    vertexShader_id = glCreateShader(GL_VERTEX_SHADER);            // initialise the vertex shader with id
    glShaderSource(vertexShader_id, 1, &vertexShaderSource, NULL); // set the vertex shader id's source glsl code
    glCompileShader(vertexShader_id);                              // compile the shader code

    // check for success
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    return vertexShader_id;
}

// fragment shader
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

// set up the fragment shader
unsigned int setUpFragmentShader()
{
    unsigned int fragmentShader_id;
    fragmentShader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_id, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader_id);

    // check for success
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    return fragmentShader_id;
}

// set up the shader program
unsigned int setUpShaderProgram(unsigned int vertexShader_id, unsigned int fragmentShader_id)
{
    unsigned int shaderProgram_id = glCreateProgram();
    glAttachShader(shaderProgram_id, vertexShader_id);
    glAttachShader(shaderProgram_id, fragmentShader_id);
    glLinkProgram(shaderProgram_id);
    // check for success
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
                  << infoLog << std::endl;
    }
    return shaderProgram_id;
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

    // set up vao
    unsigned int vao_id = setUpVAO();
    setUpTriangle();

    // set up our shader program
    unsigned int shaderProgram_id = setUpShaderProgram(setUpVertexShader(), setUpFragmentShader());

    // tell open gl how to interpret vertex buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0); // enable vertex attribute 0

    // keep doing this loop until user wants to close
    while (!glfwWindowShouldClose(window))
    {

        // we clear the screen with a blue color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // we clear the color buffer

        glUseProgram(shaderProgram_id); // use the shader program we set up
        glBindVertexArray(vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        processInput(window); // process input events

        glfwSwapBuffers(window); // swap the buffer we have been drawing to into the front
        glfwPollEvents();        // poll for events
    }

    glfwTerminate();
    return 0;
}