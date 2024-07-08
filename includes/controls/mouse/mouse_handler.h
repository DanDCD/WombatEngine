#pragma once
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <memory>

class MouseHandler
{
public:
    /// @brief constructor
    /// @param screenDims the dimensions of the screen
    MouseHandler(std::shared_ptr<GLFWwindow> window, glm::vec2 &screenDims);

    /// @brief enable the mouse capture (hide and lock it)
    void enable();

    /// @brief disable the mouse capture (show and unlock it)
    void disable();

    /// @brief check if Mouse handling is enabled
    /// @return if mouse handling is enabled
    bool isEnabled();

private:
    /// @brief if the mouse should be active or not
    bool activeMouse;

    /// @brief if this is the first frame we are tracking the mouse position
    bool firstMouse;

    /// @brief the last recorded mouse position
    glm::vec2 lastMousePosition;

    /// @brief the GLFW window in which we are capturing the mouse
    std::shared_ptr<GLFWwindow> window;

};