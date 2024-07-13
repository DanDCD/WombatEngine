#include "utils/delta_tracker/delta_tracker.h"
#include <GLFW/glfw3.h>

DeltaTracker::DeltaTracker()
    : lastFrameTime(0)
{
}

float DeltaTracker::getDelta()
{
    float currentTime = glfwGetTime();
    // if we havent recorded a frame yet (i.e. this is the first frame)
    if(lastFrameTime == 0)
    {
        lastFrameTime = currentTime;
        return 0;
    }
    
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    return deltaTime;
}