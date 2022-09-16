#pragma once

#include "Direction.hpp"
#include "Location.hpp"

#include <memory>
#include <string>

class Vehicle {
    friend class Processor;

public:
    explicit Vehicle(const std::string& id, Location location, LengthUnit size, Direction direction, LengthUnit speed)
        : mId(id)
        , mLocation(location)
        , mDirection(direction)
        , mSpeed(speed)
        , mSize(size)
    {
    }

    const std::string& getId() const { return mId; }

    Location getLocation() const { return mLocation; }
    Direction getDirection() const { return mDirection; }

    LengthUnit getSize() const { return mSize; }

private:
    std::string mId;
    Location mLocation;
    Direction mDirection;
    LengthUnit mSpeed;
    LengthUnit mSize;
};
