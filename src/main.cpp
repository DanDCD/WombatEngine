
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <rendering/shader/shader.h>
#include <vert_data/vert_data.h>
#include <stb/stb_image.h>
#include <rendering/texture/texture.h>
#include "rendering/vao/vao.h"
#include "rendering/buffer/vbo/vbo.h"
#include "rendering/buffer/ebo/ebo.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering/camera/camera.h"
#include "rendering/render_consts.h"
#include <memory>
#include "utils/signal/signal/signal.h"
#include "input/mouse_tracker/mouse_tracker.h"
#include "rendering/camera/camera.h"
#include "input/key_tracker/key_tracker.h"

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

bool M_was_pressed = false; // TODO: clean this up in a seperate input handler
bool isActive = false;
// function to process input events from user
void processInput(GLFWwindow *window)
{
    // if user presses escape, we tell GLFW we want to close the given window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // if user presses M, we tell GLFW to toggle mouse
    bool M_is_pressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;

    if (M_is_pressed)
    {
        if (!M_was_pressed)
        {
            // toggle mouse handling
            if (isActive)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            isActive = !isActive;
            std::cout << "Pressed M!" << std::endl;
        }
    }
    M_was_pressed = M_is_pressed;
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
    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "ThreeDimSim", NULL, NULL), glfwDestroyWindow);
    if (window == NULL)
    {
        std::cout << "Failed to create a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window.get());

    // Initialise GLAD
    bool loaded_GLAD = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // tell GLAD to load the address of the OpenGL function pointers
    if (!loaded_GLAD)
    {
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // enable depth TODO: Update comment

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // tell OpenGL the size of the rendering Window
    glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
    // assign our resizing function as the resizing window callback for our window
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);

    // Set Up Rendering
    Shader shader("shaders/test_vertex.vert", "shaders/test_fragment.frag");

    VAO vao = VAO();

    VBO rectVBO = VBO(GL_ARRAY_BUFFER);
    rectVBO.assignData(VERT_DATA::vertices, sizeof(VERT_DATA::vertices), GL_STATIC_DRAW);

    EBO rectEBO = EBO();
    rectEBO.assignData(VERT_DATA::indices, sizeof(VERT_DATA::indices), GL_STATIC_DRAW);

    VertexBufferLayout layout = VertexBufferLayout();
    layout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE); // vertex local position
    layout.addAttribute(GL_FLOAT, 2, 2 * sizeof(float), GL_FALSE); // texture position

    vao.addBuffer(std::move(rectVBO), layout);
    vao.addBuffer(std::move(rectEBO));

    Texture texture_1(GL_TEXTURE_2D,
                      {TextureParam(GL_TEXTURE_WRAP_S, GL_REPEAT),
                       TextureParam(GL_TEXTURE_WRAP_T, GL_REPEAT),
                       TextureParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
                       TextureParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR)},
                      "textures/container.jpg",
                      GL_TEXTURE0);

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

    // Setup Camera
    CameraParams cameraParams(glm::vec3(0.0f, 0.0f, 3.0f), 0.0f, 0.0f, 2.0f, 0.1f, 45.0f);
    Camera camera = Camera(cameraParams);

    // Input Handling
    MouseTracker::initialise(window);
    SignalHandler<MouseData> mouseHandler([&camera](MouseData mouseData)
                                          { camera.processMouseMovement(mouseData.offset_from_last.x, mouseData.offset_from_last.y); });
    MouseTracker::getOnMouseMovedSignal().addHandler(mouseHandler);

    KeyTracker::initialise(window);
    SignalHandler<KeyData> keyHandler([](KeyData KeyData)
                                      {
        if(KeyData.action == GLFW_RELEASE) 
        {
            std::cout << "key released. key: " << KeyData.key_code << " duration: " << KeyData.hold_duration << std::endl;
        } });
    KeyTracker::getOnKeyPressedSignal().addHandler(keyHandler);

    // we only have to set these uniforms once!
    shader.use();
    shader.setUniform("texture_1", 0); // texture1 is in GL_TEXTURE0

    // keep doing this loop until user wants to close
    while (!glfwWindowShouldClose(window.get()))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        processInput(window.get());

        // imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        // get view matrix
        glm::mat4 view = camera.getViewMatrix();

        // projection matrix
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SRC_WIDTH / (float)SRC_HEIGHT, 0.1f, 100.0f);

        shader.use();
        shader.setUniform("view", 1, false, view);             // set the view matrix
        shader.setUniform("projection", 1, false, projection); // set the projection matrix

        texture_1.bind();

        vao.bind();
        for (unsigned int i = 0; i < 10; i++)
        {
            // model matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 1.0f + 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setUniform("model", 1, false, model);
            glDrawElements(GL_TRIANGLES, sizeof(VERT_DATA::indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window.get()); // swap the buffer we have been drawing to into the front
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}