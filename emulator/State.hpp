#pragma once

#include <algorithm>
#include <vector>

#include "Car.hpp"
#include "Direction.hpp"
#include "Location.hpp"
#include "Road.hpp"
#include "Unit.hpp"
#include "Vehicle.hpp"

using VehicleList = std::vector<Vehicle>;

class State {
public:
    explicit State(TimeUnit currentTime, TimeUnit timeLimit, Car& car, const Road& road, //
        const VehicleList& visibleVehicles, bool collision, bool destination)
        : mCurrentTime(currentTime)
        , mTimeLimit(timeLimit)
        , mCar(car)
        , mRoad(road)
        , mVisibleVehicles(visibleVehicles)
        , mCollision(collision)
        , mDestination(destination)
    {
    }

    TimeUnit getCurrentTime() const { return mCurrentTime; }
    TimeUnit getTimeLimit() const { return mTimeLimit; }

    Car& getCar() const { return mCar; }
    const Road& getRoad() const { return mRoad; }

    const VehicleList& getVisibleVehicles() const { return mVisibleVehicles; }

    bool isCollision() const { return mCollision; }
    bool isDestination() const { return mDestination; }

private:
    TimeUnit mCurrentTime;
    TimeUnit mTimeLimit;
    Car& mCar;
    const Road& mRoad;
    const VehicleList& mVisibleVehicles;
    bool mCollision;
    bool mDestination;
};
