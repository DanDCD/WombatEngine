#include "controls/mouse/mouse_handler.h"


MouseHandler::MouseHandler(std::shared_ptr<GLFWwindow> window, glm::vec2 &screenDims)
    : activeMouse(false), firstMouse(true), window(window)
{
    lastMousePosition.x = screenDims.x / 2.0f;
    lastMousePosition.y = screenDims.y / 2.0f;
}

void MouseHandler::enable()
{
    activeMouse = true;
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void MouseHandler::disable()
{
    activeMouse = false;
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool MouseHandler::isEnabled() {
    return activeMouse;
}