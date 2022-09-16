#pragma once

#include <memory>
#include <utility>

#include "Location.hpp"
#include "Unit.hpp"

class Lane {
public:
    explicit Lane(Location location, Direction direction)
        : mLocation(location)
        , mDirection(direction)
    {
    }

    Location getLocation() const { return mLocation; }
    Direction getDirection() const { return mDirection; }

private:
    Location mLocation;
    Direction mDirection;
};
