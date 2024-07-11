#include "input/key_tracker/key_tracker.h"

void KeyTracker::initialise(std::shared_ptr<GLFWwindow> window)
{
    getInstance().trackedWindow = window;
    getInstance().onKeyPressedSignal = Signal<KeyData>();
    getInstance().lastKeyPressTimeStamps = std::unordered_map<int, double>();
    glfwSetKeyCallback(getInstance().trackedWindow.get(), KeyTracker::keyCallBack);
}

Signal<KeyData> &KeyTracker::getOnKeyPressedSignal()
{
    return getInstance().onKeyPressedSignal;
}

KeyTracker &KeyTracker::getInstance()
{
    static KeyTracker instance;
    return instance;
}

KeyTracker::KeyTracker() {}

void KeyTracker::keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // populate basic KeyData attributes
    KeyData keyData;
    keyData.key_code = key;
    keyData.scancode = scancode;
    keyData.action = action;
    keyData.mods = mods;
    keyData.timestamp = glfwGetTime();
    keyData.hold_duration = 0;

    // check and update history
    if (keyData.action == GLFW_PRESS) // if we have just pressed this key start timing its press duration
        getInstance().lastKeyPressTimeStamps[keyData.key_code] = keyData.timestamp;
    else if (keyData.action == GLFW_RELEASE) // if we have instead let a key go calculate and update the hold duration
    {
        // if the key press has been recorded in lastKeyPresTimeStamps
        if (getInstance().lastKeyPressTimeStamps.find(keyData.key_code) != getInstance().lastKeyPressTimeStamps.end())
        {
            keyData.hold_duration = keyData.timestamp - getInstance().lastKeyPressTimeStamps[keyData.key_code];
            getInstance().lastKeyPressTimeStamps.erase(keyData.key_code);
        }
    }

    getInstance().updateKeyData(keyData);
}

void KeyTracker::updateKeyData(KeyData new_data)
{
    getInstance().lastCapturedData = new_data;
    getInstance().onKeyPressedSignal.emit(new_data);
}