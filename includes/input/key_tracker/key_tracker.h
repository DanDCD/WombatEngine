#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include "utils/signal/signal/signal.h"
#include <glm/glm.hpp>
#include <optional>
#include <unordered_map>

struct KeyData
{
    /// @brief
    int key_code;

    /// @brief
    int scancode;

    /// @brief
    int action;

    /// @brief
    int mods;

    /// @brief
    double timestamp;

    /// @brief
    float hold_duration;
};

/// @brief
class KeyTracker
{
public:
    /// @brief initialise the KeyTracker singleton
    /// @param window the GLFW window to track the keys in
    static void initialise(std::shared_ptr<GLFWwindow> window);

    /// @brief obtain a Signal that is emitted whenever a key is pressed or released
    /// @return a Signal that emits a KeyData struct on key press
    static Signal<KeyData> &getOnKeyEventSignal();

    /// @brief obtain a signal that is emitted for each key that is held while pollKeyEvents is called
    /// @return a Signal that emits a KeyData struct
    static Signal<KeyData> &getOnKeyHeldSignal();

    /// @brief check if key is currently being pressed
    /// @param key_code the code of the key
    /// @return if the key is currently held down
    static bool isKeyPressed(int key_code);

    /// @brief fetch data for pressed key
    /// @param key_code the code of the key to query
    /// @return an optional object - will be empty if the key is not pressed - will return data if key has been pressed
    static std::optional<KeyData> getKeyData(int key_code);

    /// @brief polls for currently pressed keys and emits the onKeyHeldSignal for each
    static void pollKeyEvents();

    // delete the copy constructor
    KeyTracker(KeyTracker const &) = delete;
    // delete the copy assignement operator
    void operator=(KeyTracker const &) = delete;

private:
    // prevent construction
    KeyTracker();

    /// @brief returns the only instance of this singleton
    /// @return reference to this singleton
    static KeyTracker &getInstance();

    /// @brief the callback for a key event
    /// @param window the tracked window
    /// @param key the number of the key pressed
    /// @param scancode the platform specific scan code of the key
    /// @param action the action (GL_PRESS/GL_RELEASE/GL_REPEAT)
    /// @param mods modifier bits for if the key was pressed with alt/shift/etc.
    static void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods);

    /// @brief a pointer to the window of which the mouse is tracked
    std::shared_ptr<GLFWwindow> trackedWindow;

    /// @brief signal that emits key data when a key is pressed or released
    Signal<KeyData> onKeyEvent;

    /// @brief signal that emits key data when a key is held and pollKeyEvents() is called
    Signal<KeyData> onKeyHeld;

    /// @brief a map of key codes to KeyData for all currently pressed keys
    std::unordered_map<int, KeyData> pressedKeys;
};