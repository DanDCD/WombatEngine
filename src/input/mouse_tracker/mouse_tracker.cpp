#include "input/mouse_tracker/mouse_tracker.h"
#include <iostream>
#include "utils/logging/logging.h"

void MouseTracker::initialise(std::shared_ptr<GLFWwindow> window)
{
    getInstance().trackedWindow = window;
    getInstance().onMouseMovedSignal = Signal<MouseData>();
    glfwSetCursorPosCallback(getInstance().trackedWindow.get(), MouseTracker::cursorPosCallback);
    LOG("Initialised Mouse Tracking", Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
}

Signal<MouseData> &MouseTracker::getOnMouseMovedSignal()
{
    return getInstance().onMouseMovedSignal;
}

void MouseTracker::updateMouseData(MouseData new_data)
{
    getInstance().lastCapturedData = new_data;
    getInstance().onMouseMovedSignal.emit(new_data);
}

MouseTracker &MouseTracker::getInstance()
{
    static MouseTracker instance;
    return instance;
}

MouseTracker::MouseTracker() {}

void MouseTracker::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    // set current position
    MouseData mouseData;
    mouseData.position = glm::vec2((float)xpos, (float)ypos);

    // calculate offset
    glm::vec2 offset;
    if (getInstance().lastCapturedData.has_value())
    {
        MouseData lastMouseData = getInstance().lastCapturedData.value();
        offset.x = mouseData.position.x - lastMouseData.position.x;
        offset.y = -(mouseData.position.y - lastMouseData.position.y); // reversed as y-axis is bottom-up
    }
    else
        offset = glm::vec2(0.0f, 0.0f);

    mouseData.offset_from_last = offset;
    mouseData.motion_direction = glm::normalize(mouseData.offset_from_last);

    MouseTracker::updateMouseData(mouseData);
}