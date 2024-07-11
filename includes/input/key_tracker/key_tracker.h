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

    /// @brief obtain a Signal that is emitted whenever a key is pressed
    /// @return a Signal that emits a KeyData struct on key press
    static Signal<KeyData> &getOnKeyPressedSignal();

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
    static void keyCallBack(GLFWwindow *window,  int key, int scancode, int action, int mods);

    /// @brief update the tracked data
    /// @param new_data the new data
    static void updateKeyData(KeyData new_data);

    /// @brief a pointer to the window of which the mouse is tracked
    std::shared_ptr<GLFWwindow> trackedWindow;

    /// @brief signal that emits key data when a key is pressed
    Signal<KeyData> onKeyPressedSignal;

    /// @brief the last captured key data - will be empty if no key has been pressed yet
    std::optional<KeyData> lastCapturedData;

    /// @brief a map of key code to the timestamp it was last pressed at
    std::unordered_map<int, double> lastKeyPressTimeStamps;
};