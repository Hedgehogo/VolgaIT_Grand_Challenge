cmake_minimum_required(VERSION 3.23)
project(VolgaIT_Grand_Challenge)

set(CMAKE_CXX_STANDARD 17)

add_executable(VolgaIT_Grand_Challenge "main.cpp"
        "emulator/Car.hpp"
        "emulator/Circle.hpp"
        "emulator/Direction.hpp"
        "emulator/Emulator.hpp"
        "emulator/Emulator.cpp"
        "emulator/Gear.hpp"
        "emulator/Lane.hpp"
        "emulator/Loader.hpp"
        "emulator/Loader.cpp"
        "emulator/Location.hpp"
        "emulator/Mode.hpp"
        "emulator/Processor.hpp"
        "emulator/Processor.cpp"
        "emulator/Road.hpp"
        "emulator/State.hpp"
        "emulator/TrafficSign.hpp"
        "emulator/Unit.hpp"
        "emulator/Vehicle.hpp"
        work/Application/application.cpp work/Application/application.hpp)
