#pragma once

#include "Processor.hpp"
#include "State.hpp"

#include <fstream>

class Loader {
public:
    explicit Loader(const std::string& fileName);

    Processor load();

private:
    TimeUnit loadTimeLimit();
    LaneList loadLanes();
    TrafficSignList loadTrafficSigns();
    Road loadRoad();
    Car loadCar();
    VehicleList loadVehicles();

private:
    std::ifstream mFile;
};
