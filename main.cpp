#include "emulator/Emulator.hpp"
#include "work/Application/application.hpp"

#include <conio.h>
#include <windows.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

int main(int argc, char** argv)
{
    try {
        Emulator emulator(argc, argv);
        Application app(emulator.getMode());
        emulator.run([&app](State& state) { return app.run(state); });
        return 0;
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
}
