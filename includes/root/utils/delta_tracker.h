#pragma once

class DeltaTracker
{
public:
    DeltaTracker();

    float getDelta();

private:
    float lastFrameTime;
}; 