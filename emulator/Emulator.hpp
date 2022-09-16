#pragma once

#include "Mode.hpp"
#include "State.hpp"

#include <chrono>
#include <functional>
#include <string>

class Emulator {
public:
    explicit Emulator(int argc, char** argv);

    Mode getMode() const;
    
    void run(std::function<bool(State& state)> callback);

private:
    Mode mMode;
    std::chrono::milliseconds mDelay;
};
