#pragma once

#include "Direction.hpp"
#include "Gear.hpp"
#include "Location.hpp"
#include "Circle.hpp"

#include <algorithm>
#include <string>

class Car {
    friend class Processor;

public:
    struct Properties {
        LengthUnit size;
        LengthUnit visibleRange;

        LengthUnit minSpeed;
        LengthUnit forwardMaxSpeed;
        LengthUnit reverseMaxSpeed;

        LengthUnit forwardAccelerationRate;
        LengthUnit reverseAccelerationRate;
        LengthUnit decelerationRate;

        AngleUnit rotationRate;
    };

    explicit Car(Location location, Direction direction, Circle destination, Properties properties)
        : mLocation(location)
        , mDirection(direction)
        , mProperties(std::move(properties))
        , mDestination(destination)
    {
    }

    Location getLocation() const { return mLocation; }
    Circle getDestination() const { return mDestination; }
    LengthUnit getSize() const { return mProperties.size; }

    Direction getDirection() const { return mDirection; }

    LengthUnit getMinSpeed() const { return mProperties.minSpeed; }

    LengthUnit getMaxSpeed(Gear gear) const
    {
        switch (gear) {
        case Gear::Forward:
            return mProperties.forwardMaxSpeed;
        case Gear::Reverse:
            return mProperties.reverseMaxSpeed;
        default:
            return 0;
        }
    }

    LengthUnit getCurrentSpeed() const { return mSpeed; }

    void turnSteeringWheel(AngleUnit angle) { mSteeringWheelAngle = std::clamp(angle, -1., 1.); }

    void setGear(Gear gear) { mGear = gear; }
    void pressGasPedal(ForceUnit force) { mGasPedalForce = std::clamp(force, 0., 1.); }
    void pressBrakePedal(ForceUnit force) { mBrakePedalForce = std::clamp(force, 0., 1.); }

private:
    Location mLocation;
    Circle mDestination;
    Properties mProperties;

    LengthUnit mSpeed = 0;
    Direction mDirection;

    Gear mGear = Gear::Neutral;
    AngleUnit mSteeringWheelAngle = 0;
    ForceUnit mGasPedalForce = 0;
    ForceUnit mBrakePedalForce = 0;
};
