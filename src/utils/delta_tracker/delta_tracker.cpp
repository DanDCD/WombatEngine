#include "utils/delta_tracker/delta_tracker.h"
#include <GLFW/glfw3.h>

DeltaTracker::DeltaTracker()
    : lastFrameTime(0)
{
}

float DeltaTracker::getDelta()
{
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    return deltaTime;
}