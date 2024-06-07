
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <shader/shader.h>


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
    0.0f, 0.5f, 0.0f
};

float colored_triangle_verts[] = {
    // positions         colors
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f
};


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
    glBufferData(GL_ARRAY_BUFFER, sizeof(colored_triangle_verts), colored_triangle_verts, GL_STATIC_DRAW);
}

float triangles_verts[] = {
    -0.5f, 0.5f, 0.0f, //0
    0.0f, 0.5f, 0.0f, //1
    -0.25f, -0.5f, 0.0f, //2
    0.5f, 0.5f, 0.0f, //3
    0.25f, -0.5f, 0.0f //4
};

unsigned int triangles_indices[] = {
    // clockwise
    2, 0, 1, // first triangle
    1, 3, 4 // second triangle
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

// set up our rectangle - conversely, we use EBO and index drawing to save memory!
void setUpRectangle()
{
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_verts), rectangle_verts, GL_STATIC_DRAW);

    unsigned int ebo_id;
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices), rectangle_indices, GL_STATIC_DRAW);
}

// vertex shader
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "out vec3 ourColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   ourColor = aColor;\n"
                                 "   gl_Position = vec4(aPos, 1.0);\n"
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
                                   "in vec3 ourColor;\n" 
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(ourColor, 1.0);\n"
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
    Shader myShader;
    myShader.helloWorld();

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
    // setUpRectangle();

    // set up our shader program
    unsigned int shaderProgram_id = setUpShaderProgram(setUpVertexShader(), setUpFragmentShader());

    // tell open gl how to interpret vertex buffer data
    // for vertext attribute 0, we have 3 float values for each attrib, stride of six, we do not start with an offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0); // enable vertex attribute 0
    glEnableVertexAttribArray(1);

    // keep doing this loop until user wants to close
    while (!glfwWindowShouldClose(window))
    {

        // we clear the screen with a blue color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // we clear the color buffer

        glUseProgram(shaderProgram_id); // use the shader program we set up

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