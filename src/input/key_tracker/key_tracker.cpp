#include "input/key_tracker/key_tracker.h"

void KeyTracker::initialise(std::shared_ptr<GLFWwindow> window)
{
    getInstance().trackedWindow = window;
    getInstance().onKeyEvent = Signal<KeyData>();
    getInstance().pressedKeys = std::unordered_map<int, KeyData>();
    glfwSetKeyCallback(getInstance().trackedWindow.get(), KeyTracker::keyCallBack);
    LOG("Initialised Key Tracking", Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
}

Signal<KeyData> &KeyTracker::getOnKeyEventSignal()
{
    return getInstance().onKeyEvent;
}

Signal<KeyData> &KeyTracker::getOnKeyHeldSignal()
{
    return getInstance().onKeyHeld;
}

bool KeyTracker::isKeyPressed(int key_code)
{
    return (getInstance().pressedKeys.find(key_code) != getInstance().pressedKeys.end());
}

std::optional<KeyData> KeyTracker::getKeyData(int key_code)
{
    if (!isKeyPressed)
        return std::nullopt;
    return getInstance().pressedKeys[key_code];
}

void KeyTracker::pollKeyEvents()
{
    for (const auto &pair : getInstance().pressedKeys)
    {
        KeyData keyData = pair.second;
        keyData.action = GLFW_REPEAT;
        keyData.hold_duration = glfwGetTime() - keyData.timestamp;
        getInstance().onKeyHeld.emit(keyData);
    }
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

    if (keyData.action == GLFW_PRESS)
    {
        // if a key is newly pressed we add it to the history (and keep hold_duration=0)
        if (!isKeyPressed(keyData.key_code)) 
            getInstance().pressedKeys[keyData.key_code] = keyData;
        // if key pressed event but its already in our history we dont want to emit a signal
        else 
            return;
    }
    // if a key has been pressed and is now released, we calculate the hold duration and remove it from history
    else if (isKeyPressed(keyData.key_code) && keyData.action == GLFW_RELEASE)
    {
        keyData.hold_duration = keyData.timestamp - getInstance().pressedKeys[keyData.key_code].timestamp;
        getInstance().pressedKeys.erase(keyData.key_code);
    }
    // emit our onKeyPressed signal
    getInstance().onKeyEvent.emit(keyData);
}
