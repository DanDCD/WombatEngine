#pragma once
#include <functional>

template <typename T>
class SignalHandler
{

public:
    /// @brief default constructor
    SignalHandler()
        : callback()
    {
    }

    /// @brief constructor with callback initialisation
    /// @param callback a callback function that expects T as input and returns nothing
    SignalHandler(std::function<void(T)> callback)
        : callback(callback)
    {
    }

    /// @brief
    /// @param data
    void receiveSignal(T data)
    {
        if (callback)
            callback(data);
    }

    /// @brief
    /// @param new_callback
    void setCallback(std::function<void(T)> new_callback)
    {
        callback = new_callback;
    }

private:
    std::function<void(T)> callback;
};
