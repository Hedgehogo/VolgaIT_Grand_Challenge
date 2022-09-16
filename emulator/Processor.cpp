#include "Processor.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>

Processor::Processor(Road&& road, Car&& car, VehicleList&& vehicles, TimeUnit timeLimit)
    : mRoad(std::move(road))
    , mCar(std::move(car))
    , mVehicles(std::move(vehicles))
    , mTimeLimit(timeLimit)
{
}

void Processor::delay() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }

bool Processor::run(std::function<bool(State& state)> callback, std::chrono::milliseconds delay)
{
    VehicleList visibleVehicles;
    bool collision = false;

    for (TimeUnit i = 0; i < mTimeLimit; ++i) {
        updateVisibleVehicles(visibleVehicles);

        const bool destination = checkDestination();
        State state(i, mTimeLimit, mCar, mRoad, visibleVehicles, collision, destination);

        if (!callback(state))
            return false;

        if (delay.count())
            std::this_thread::sleep_for(delay);

        handleUserActions();
        handleOtherActions();

        updateLocations();

        collision = !checkCollisions();
    }

    return true;
}

void Processor::updateVisibleVehicles(VehicleList& visibleVehicles)
{
    visibleVehicles.clear();

    for (const auto& vehicle : mVehicles) {
        if (mCar.mLocation.isInRange(vehicle.mLocation, mCar.mProperties.visibleRange))
            visibleVehicles.emplace_back(vehicle);
    }
}

void Processor::handleUserActions()
{
    const auto& properties = mCar.mProperties;

    auto speed = mCar.mSpeed;

    switch (mCar.mGear) {
    case Gear::Forward:
        speed += mCar.mGasPedalForce * properties.forwardAccelerationRate;
        speed = std::min(speed, properties.forwardMaxSpeed * mCar.mGasPedalForce);
        break;

    case Gear::Reverse:
        speed -= mCar.mGasPedalForce * properties.reverseAccelerationRate;
        speed = std::max(speed, -properties.reverseMaxSpeed * mCar.mGasPedalForce);
        break;
    }

    const auto slowdown = mCar.mBrakePedalForce * properties.decelerationRate;

    if (speed > slowdown)
        speed -= slowdown;
    else if (speed < -slowdown)
        speed += slowdown;
    else
        speed = 0;

    mCar.mSpeed = speed;

    mCar.mDirection = mCar.mDirection.rotated(
        mCar.mSteeringWheelAngle * std::min(std::abs(speed) / properties.minSpeed, 1.0) * properties.rotationRate);
}

void Processor::handleOtherActions() { }

void Processor::updateLocations()
{
    mCar.mLocation = mCar.mLocation.moved(mCar.mDirection, mCar.mSpeed);

    for (auto& vehicle : mVehicles)
        vehicle.mLocation = vehicle.mLocation.moved(vehicle.mDirection, vehicle.mSpeed);
}

bool Processor::checkCollisions()
{
    for (const auto& vehicle : mVehicles) {
        if (mCar.mLocation.isInRange(vehicle.mLocation, (mCar.mProperties.size + vehicle.mSize) / 2.))
            return false;
    }

    for (const auto& lane : mRoad.getLanes()) {
        const auto direction = lane.getDirection();
        const auto startPoint = lane.getLocation();
        const Location endPoint = { startPoint.x + direction.dx, startPoint.y + direction.dy };

        const auto location = mCar.getLocation();
        const auto size = 0;

        if (startPoint.x <= location.x - size && location.x + size <= endPoint.x && //
            startPoint.y <= location.y - size && location.y + size <= endPoint.y) {

            return true;
        }
    }

    return false;
}

bool Processor::checkDestination() //
{
    return mCar.mLocation.isInRange(mCar.mDestination.center, mCar.mDestination.radius / 2) && mCar.mSpeed == 0;
}
