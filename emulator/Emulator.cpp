#include "Emulator.hpp"

#include "Loader.hpp"

#include <fstream>

Emulator::Emulator(int argc, char** argv)
{
    std::ifstream file("mode.txt");
    unsigned int mode = 0, delay = 0;
    file >> mode >> delay;
    mMode = mode ? Mode::HandsOn : Mode::HandsOff;
    mDelay = std::chrono::milliseconds(delay);
}

Mode Emulator::getMode() const { return mMode; }

void Emulator::run(std::function<bool(State& state)> callback)
{
    Loader loader("input.txt");
    auto processor = loader.load();
    processor.run(callback, mDelay);
}
