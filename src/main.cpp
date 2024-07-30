
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

int main()
{

    Assimp::Importer importer;

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
    Shader rectShaderProgram("shaders/test_vertex_no_texture.vert", "shaders/test_fragment_no_texture.frag");
    Shader lightSourceShaderProgram("shaders/test_vertex_no_texture.vert", "shaders/test_fragment_light_source.frag");

    // set up container model VAO
    VAO rectVAO = VAO();

    VBO rectVBO = VBO(GL_ARRAY_BUFFER);
    rectVBO.assignData(VERT_DATA::vertices_no_tex_coord, sizeof(VERT_DATA::vertices_no_tex_coord), GL_STATIC_DRAW);

    EBO rectEBO = EBO();
    rectEBO.assignData(VERT_DATA::indices, sizeof(VERT_DATA::indices), GL_STATIC_DRAW);

    VertexBufferLayout rectLayout = VertexBufferLayout();
    rectLayout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE); // vertex local position
    // layout.addAttribute(GL_FLOAT, 2, 2 * sizeof(float), GL_FALSE); // texture position

    rectVAO.addBuffer(std::move(rectVBO), rectLayout);
    rectVAO.addBuffer(std::move(rectEBO));

    glm::vec3 rectColor = glm::vec3(0.15f, 0.24f, 0.95f);

    // set up light source vao (note: we use same model as container right now)
    VAO lightSourceVAO = VAO();

    VBO lightSourceVBO = VBO(GL_ARRAY_BUFFER);
    lightSourceVBO.assignData(VERT_DATA::vertices_no_tex_coord, sizeof(VERT_DATA::vertices_no_tex_coord), GL_STATIC_DRAW);

    EBO lightSourceEBO = EBO();
    lightSourceEBO.assignData(VERT_DATA::indices, sizeof(VERT_DATA::indices), GL_STATIC_DRAW);

    VertexBufferLayout lightSourceLayout = VertexBufferLayout();
    lightSourceLayout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE); // vertex local position

    lightSourceVAO.addBuffer(std::move(lightSourceVBO), lightSourceLayout);
    lightSourceVAO.addBuffer(std::move(lightSourceEBO));

    glm::vec3 lightSourceColourEmission = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightSourceColour = glm::vec3(1.0f, 1.0f, 1.0f);

    // Texture texture_1(GL_TEXTURE_2D,
    //                   {TextureParam(GL_TEXTURE_WRAP_S, GL_REPEAT),
    //                    TextureParam(GL_TEXTURE_WRAP_T, GL_REPEAT),
    //                    TextureParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
    //                    TextureParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR)},
    //                   "textures/container.jpg",
    //                   GL_TEXTURE0);

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

    // light source position
    glm::vec3 lightSourcePosition = glm::vec3(-5.0f, -5.0f, 0.0f);

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

    // we only have to set these uniforms once!
    rectShaderProgram.use();
    // rectShaderProgram.setUniform("texture_1", 0); // texture1 is in GL_TEXTURE0

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

        // bind and draw lightsource
        lightSourceShaderProgram.use();
        lightSourceShaderProgram.setUniform("view", 1, false, view);
        lightSourceShaderProgram.setUniform("projection", 1, false, projection);

        lightSourceVAO.bind();

        glm::mat4 lightSourceModel = glm::mat4(1.0f);
        lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition);
        lightSourceShaderProgram.setUniform("model", 1, false, lightSourceModel);
        glDrawElements(GL_TRIANGLES, sizeof(VERT_DATA::indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        // bind and draw containers
        // texture_1.bind();
        rectShaderProgram.use();
        rectShaderProgram.setUniform("view", 1, false, view);             // set the view matrix
        rectShaderProgram.setUniform("projection", 1, false, projection); // set the projection matrix

        rectVAO.bind();
        for (unsigned int i = 0; i < 10; i++)
        {
            // model matrix for containers
            glm::mat4 rectModel = glm::mat4(1.0f);
            rectModel = glm::translate(rectModel, cubePositions[i]);
            float angle = 1.0f + 20.0f * i;
            rectModel = glm::rotate(rectModel, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            rectShaderProgram.setUniform("model", 1, false, rectModel);
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