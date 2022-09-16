#pragma once

#include "Unit.hpp"

class TrafficSign {
public:
    enum Type {
        MinimumSpeedLimit,
        MaximumSpeedLimit,
    };

    explicit TrafficSign(Type type, std::size_t laneIndex, LengthUnit location, LengthUnit value)
        : mType(type)
        , mLaneIndex(laneIndex)
        , mLocation(location)
        , mValue(value)
    {
    }

    Type getType() const { return mType; }
    LengthUnit getValue() const { return mValue; }

    std::size_t getLaneIndex() const { return mLaneIndex; }
    LengthUnit getLocation() const { return mLocation; }

private:
    Type mType;
    LengthUnit mValue;
    std::size_t mLaneIndex;
    LengthUnit mLocation;
};
