#include "input/key_tracker/key_tracker.h"

void KeyTracker::initialise(std::shared_ptr<GLFWwindow> window)
{
    getInstance().trackedWindow = window;
    getInstance().onKeyPressedSignal = Signal<KeyData>();
    getInstance().pressedKeys = std::unordered_map<int, KeyData>();
    glfwSetKeyCallback(getInstance().trackedWindow.get(), KeyTracker::keyCallBack);
}

Signal<KeyData> &KeyTracker::getOnKeyPressedSignal()
{
    return getInstance().onKeyPressedSignal;
}

bool KeyTracker::isKeyPressed(int key_code)
{
    return (getInstance().pressedKeys.find(key_code) != getInstance().pressedKeys.end());
}

std::optional<KeyData> KeyTracker::getKeyData(int key_code)
{
    if (!isKeyPressed)
        return std::nullopt;
    KeyData keyData;
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

    // if a key is newly pressed we add it to the history (and keep hold_duration=0)
    if (!isKeyPressed(keyData.key_code) && keyData.action == GLFW_PRESS)
        getInstance().pressedKeys[keyData.key_code] = keyData;
    // if a key has been pressed and is now released, we calculate the hold duration and remove it from history
    else if (isKeyPressed(keyData.key_code) && keyData.action == GLFW_RELEASE)
    {
        keyData.hold_duration = keyData.timestamp - getInstance().pressedKeys[keyData.key_code].timestamp;
        getInstance().pressedKeys.erase(keyData.key_code);
    }
    // emit our onKeyPressed signal
    getInstance().onKeyPressedSignal.emit(keyData);
}
