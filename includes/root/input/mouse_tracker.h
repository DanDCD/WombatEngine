#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include "root/utils/signal/signal.h"
#include <glm/glm.hpp>
#include <optional>
#include "imgui.h"

/// @brief struct containing tracked mouse data
struct MouseData
{
    /// @brief the current position of the mouse
    glm::vec2 position;

    /// @brief the offset from the last position (i.e. the movement from t-1 to t)
    glm::vec2 offset_from_last;

    /// @brief the normalised direction of movement of the cursor from last frame
    glm::vec2 motion_direction;
};

/// @brief singleton class used to track the position of the mouse
class MouseTracker
{
public:
    /// @brief initialise the MouseTracker singleton
    /// @param window the GLFW window to track the mouse in
    static void initialise(std::shared_ptr<GLFWwindow> window);

    /// @brief obtain a Signal that is emitted whenever the mouse moves
    /// @return a Signal that emits a MouseData struct on cursor movement
    static Signal<MouseData> &getOnMouseMovedSignal();

    /// @brief update the tracked data
    /// @param new_data the new data
    static void updateMouseData(MouseData new_data);

    // delete the copy constructor
    MouseTracker(MouseTracker const &) = delete;
    // delete the copy assignement operator
    void operator=(MouseTracker const &) = delete;

private:
    // prevent construction
    MouseTracker();

    /// @brief returns the only instance of this singleton
    /// @return reference to this singleton
    static MouseTracker &getInstance();

    /// @brief the callback function tied to the trackedWindow on initialisation
    /// @param window the GLFWwindow associated with this mouse
    /// @param xpos the x position of the mouse
    /// @param ypos the y position of the mouse
    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

    /// @brief a pointer to the window of which the mouse is tracked
    std::shared_ptr<GLFWwindow> trackedWindow;

    /// @brief signal that emits mouse positional data when the cursor is moved
    Signal<MouseData> onMouseMovedSignal;

    /// @brief the last captured mouse data - will be empty if this is the mouse has yet to be captured
    std::optional<MouseData> lastCapturedData;
};