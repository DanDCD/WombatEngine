
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
#include "utils/delta_tracker/delta_tracker.h"
#include "assimp/Importer.hpp"
#include "rendering/assimp/mesh.h"
#include "rendering/assimp/model.h"
#include "rendering/log/check_gl.h"
#include "rendering/texture/texture_manager.h"
#include "utils/logging/logging.h"
#include "utils/text_reading/text_reading.h"

// A callback function to be called whenever the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // tell opengl the new window size (if changed)
}

int main()
{
    Logging::set_minimum_priority(Logging::LOG_PRIORITY::MEDIUM);
    LOG("\n" +
            readFile("text/wombat_screen.txt") +
            "\nWelcome to the Wombat Game Engine!\nCheck out the repo here: https://github.com/DanDCD/WombatEngine \n",
        Logging::LOG_TYPE::INFO,
        Logging::LOG_PRIORITY::HIGH);

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
        LOG("Failed to create GLFW window", Logging::LOG_TYPE::ERROR);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window.get());

    // Initialise GLAD
    bool loaded_GLAD = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // tell GLAD to load the address of the OpenGL function pointers
    if (!loaded_GLAD)
    {
        LOG("Failed to load GLAD", Logging::LOG_TYPE::ERROR);
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
    Shader shader("shaders/test_phong.vert", "shaders/test_phong.frag");

    // test: load model
    Model modelObj("models/backpack/backpack.obj");

    // Setup Camera
    CameraParams cameraParams(glm::vec3(0.0f, 0.0f, 3.0f), 0.0f, 0.0f, 2.0f, 0.1f, 45.0f);
    Camera camera = Camera(cameraParams);

    // Input Handling
    float delta; // the time between frames
    MouseTracker::initialise(window);
    SignalHandler<MouseData> mouseHandler([&camera](MouseData mouseData)
                                          { camera.processMouseMovement(mouseData.offset_from_last.x, mouseData.offset_from_last.y); });
    MouseTracker::getOnMouseMovedSignal().addHandler(mouseHandler);

    KeyTracker::initialise(window);
    bool mouse_active = false;
    SignalHandler<KeyData> keyEventHandler(
        [&window, &mouse_active](KeyData keyData)
        {
        if (keyData.action == GLFW_PRESS && keyData.key_code == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window.get(), true);
        if (keyData.action == GLFW_RELEASE && keyData.key_code == GLFW_KEY_M) 
        {
        glfwSetInputMode(window.get(), GLFW_CURSOR, mouse_active ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        mouse_active = !mouse_active;
        } });
    KeyTracker::getOnKeyEventSignal().addHandler(keyEventHandler);

    SignalHandler<KeyData> keyHoldHandler(
        [&camera, &delta](KeyData keyData)
        {
            if (keyData.key_code == GLFW_KEY_W)
                camera.processKeyboard(Camera::Movement::FORWARD, delta);
            if (keyData.key_code == GLFW_KEY_S)
                camera.processKeyboard(Camera::Movement::BACKWARD, delta);
            if (keyData.key_code == GLFW_KEY_A)
                camera.processKeyboard(Camera::Movement::LEFT, delta);
            if (keyData.key_code == GLFW_KEY_D)
                camera.processKeyboard(Camera::Movement::RIGHT, delta);
            if (keyData.key_code == GLFW_KEY_SPACE)
                camera.processKeyboard(Camera::Movement::UP, delta);
            if (keyData.key_code == GLFW_KEY_LEFT_SHIFT)
                camera.processKeyboard(Camera::Movement::DOWN, delta);
        });
    KeyTracker::getOnKeyHeldSignal().addHandler(keyHoldHandler);

    DeltaTracker deltaTracker;

    // we only need to set some uniforms for the guitar shader once
    shader.use();
    shader.setUniform("dirLight.direction", glm::vec3(0.1f, -1.0f, 0.1f));
    shader.setUniform("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setUniform("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.setUniform("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // keep doing this loop until user wants to close
    while (!glfwWindowShouldClose(window.get()))
    {
        delta = deltaTracker.getDelta();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        KeyTracker::pollKeyEvents();

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

        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down

        shader.use();
        shader.setUniform("view", 1, false, view);             // set the view matrix
        shader.setUniform("projection", 1, false, projection); // set the projection matrix
        shader.setUniform("model", 1, false, model);
        checkGLError("BEFORE MODEL DRAW");
        modelObj.draw(shader);

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