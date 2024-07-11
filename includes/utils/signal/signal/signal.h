#pragma once
#include <vector>
#include "utils/signal/signal_handler/signal_handler.h"

/// @brief
/// @tparam T
template <typename T>
class Signal
{

public:
    /// @brief
    Signal()
        : handlers()
    {
    }

    /// @brief
    /// @param handler
    void addHandler(SignalHandler<T> &handler)
    {
        handlers.push_back(handler);
    }


    /// @brief
    /// @param data
    void emit(T data)
    {
        for (auto &handler : handlers)
            handler.receiveSignal(data);
    }

private:
    std::vector<SignalHandler<T>> handlers;
};