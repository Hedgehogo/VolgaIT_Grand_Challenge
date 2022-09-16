#pragma once

#include "State.hpp"

#include <chrono>
#include <functional>
#include <string>

class Processor {
public:
    explicit Processor(Road&& road, Car&& car, VehicleList&& vehicles, TimeUnit timeLimit);

    bool run(std::function<bool(State& state)> callback, std::chrono::milliseconds delay);

private:
    void delay();

    void updateVisibleVehicles(VehicleList & visbleVehicles);

    void handleUserActions();
    void handleOtherActions();

    void updateLocations();
    bool checkCollisions();
    bool checkDestination();

private:
    Road mRoad;
    Car mCar;
    VehicleList mVehicles;
    TimeUnit mTimeLimit;
};
