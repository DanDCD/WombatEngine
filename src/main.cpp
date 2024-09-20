
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "root/graphics/implementation/shader.h"
#include "dependencies/stb/stb_image.h"
#include "root/graphics/implementation/texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "root/graphics/interface/camera.h"
#include "root/graphics/interface/render_consts.h"
#include <memory>
#include "root/utils/signal/signal.h"
#include "root/input/mouse_tracker.h"
#include "root/input/key_tracker.h"
#include "root/utils/delta_tracker.h"
#include "assimp/Importer.hpp"
#include "root/graphics/implementation/mesh.h"
#include "root/graphics/implementation/check_gl.h"
#include "root/utils/logging.h"
#include "root/utils/text_reading.h"
#include "dependencies/entt/entt.hpp"
#include "root/graphics/interface/resource_manager.h"
#include "root/components/transform_component.h"

/// @brief a callback for when the window is resized
/// @param window the glfw window
/// @param width the new width
/// @param height the new height
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // tell opengl the new window size (if changed)
}

/// @brief initialise glfw
/// @return a pointer to the glfw window
std::shared_ptr<GLFWwindow> init_glfw()
{
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
    }
    else
        glfwMakeContextCurrent(window.get());
    return window;
}

/// @brief initialise glad (i.e. set up functions to access glfw)
void init_glad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        LOG("Failed to load GLAD", Logging::LOG_TYPE::ERROR);
}

int main()
{
    // TODO: delete this testing code
    entt::registry registry;

    entt::entity entity = registry.create();
    // auto version = entt::to_version(entity); still need to find out what this does!

    auto &transform = registry.emplace<Wombat::ECS::TransformComponent>(entity);
    


    Logging::set_minimum_priority(Logging::LOG_PRIORITY::MEDIUM);
    LOG("\n" +
            readFile("text/wombat_screen.txt") +
            "\nWelcome to the Wombat Game Engine!\nCheck out the repo here: https://github.com/DanDCD/WombatEngine \n",
        Logging::LOG_TYPE::INFO,
        Logging::LOG_PRIORITY::HIGH);

    std::shared_ptr<GLFWwindow> window = init_glfw();
    init_glad();

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    // test for new resource manager
    Wombat::Graphics::ResourceManager resource_manager;
    entt::resource<MeshNode> mesh_node = resource_manager.load_model("models/backpack/backpack.obj");
    entt::resource<Shader> shader = resource_manager.load_shader("shaders/test_phong.vert", "shaders/test_phong.frag");

 
    // Shader shader("shaders/test_phong.vert", "shaders/test_phong.frag");
    // Model modelObj("models/backpack/backpack.obj");

    // Setup Camera
    Wombat::Graphics::CameraParams cameraParams(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 2.0f, 0.1f, 45.0f);
    Wombat::Graphics::Camera camera = Wombat::Graphics::Camera(cameraParams);

    // Input Handling
    float delta; // the time between frames
    bool mouse_active = false;
    MouseTracker::initialise(window);
    SignalHandler<MouseData> mouseHandler(
        [&camera, &mouse_active, &io](MouseData mouseData)
        {
            if (io.WantCaptureMouse)
                return;
            if (mouse_active)
                return;
            camera.processMouseMovement(mouseData.offset_from_last.x, mouseData.offset_from_last.y);
        });
    MouseTracker::getOnMouseMovedSignal().addHandler(mouseHandler);

    KeyTracker::initialise(window);
    SignalHandler<KeyData> keyEventHandler(
        [&window, &mouse_active, &io](KeyData keyData)
        {
            if (keyData.action == GLFW_PRESS && keyData.key_code == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window.get(), true);
            if (keyData.action == GLFW_RELEASE && keyData.key_code == GLFW_KEY_M)
            {
                mouse_active = !mouse_active;
                if (mouse_active)
                    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
                else
                    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
                glfwSetInputMode(window.get(), GLFW_CURSOR, mouse_active ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
                // Immediately update the last captured mouse position after toggling
                double xpos, ypos;
                glfwGetCursorPos(window.get(), &xpos, &ypos);
                MouseData mouseData;
                mouseData.position = glm::vec2((float)xpos, (float)ypos);
                mouseData.offset_from_last = glm::vec2(0.0f, 0.0f); // Reset the offset
                mouseData.motion_direction = glm::vec2(0.0f, 0.0f);
                MouseTracker::updateMouseData(mouseData);
            }
        });
    KeyTracker::getOnKeyEventSignal().addHandler(keyEventHandler);

    SignalHandler<KeyData> keyHoldHandler(
        [&camera, &delta](KeyData keyData)
        {
            if (keyData.key_code == GLFW_KEY_W)
                camera.processKeyboard(Wombat::Graphics::Camera::Movement::FORWARD, delta);
            if (keyData.key_code == GLFW_KEY_S)
                camera.processKeyboard(Wombat::Graphics::Camera::Movement::BACKWARD, delta);
            if (keyData.key_code == GLFW_KEY_A)
                camera.processKeyboard(Wombat::Graphics::Camera::Movement::LEFT, delta);
            if (keyData.key_code == GLFW_KEY_D)
                camera.processKeyboard(Wombat::Graphics::Camera::Movement::RIGHT, delta);
            if (keyData.key_code == GLFW_KEY_SPACE)
                camera.processKeyboard(Wombat::Graphics::Camera::Movement::UP, delta);
            if (keyData.key_code == GLFW_KEY_LEFT_SHIFT)
                camera.processKeyboard(Wombat::Graphics::Camera::Movement::DOWN, delta);
        });
    KeyTracker::getOnKeyHeldSignal().addHandler(keyHoldHandler);

    DeltaTracker deltaTracker;

    // we only need to set some uniforms for the guitar shader once
    shader->use();
    shader->setUniform("dirLight.direction", glm::vec3(0.1f, -1.0f, 0.1f));
    shader->setUniform("dirLight.ambient", glm::vec3(0.02f, 0.02f, 0.02f));
    shader->setUniform("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->setUniform("dirLight.specular", glm::vec3(0.50f, 0.5f, 0.5f));

    glm::vec3 lightSourcePosition = glm::vec3(0.0f, 0.0f, 0.0f);
    shader->setUniform("pointLights[0].position", glm::vec3(lightSourcePosition));
    shader->setUniform("pointLights[0].constant", 1.0f);
    shader->setUniform("pointLights[0].linear", 0.09f);
    shader->setUniform("pointLights[0].quadratic", 0.032f);
    shader->setUniform("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader->setUniform("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    shader->setUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // keep doing this loop until user wants to close
    while (!glfwWindowShouldClose(window.get()))
    {
        delta = deltaTracker.getDelta();

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
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(5.0f), glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // it's a bit too big for our scene, so scale it down

        shader->use();
        shader->setUniform("view", 1, false, view);             // set the view matrix
        shader->setUniform("projection", 1, false, projection); // set the projection matrix
        shader->setUniform("model", 1, false, model);
        shader->setUniform("viewPos", camera.getPosition());
        shader->setUniform("normalModel", 1, false, glm::inverse(glm::transpose(glm::mat3(model))));
        checkGLError("BEFORE MODEL DRAW");
        mesh_node->draw(shader);

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window.get()); // swap the buffer we have been drawing to into the front

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}