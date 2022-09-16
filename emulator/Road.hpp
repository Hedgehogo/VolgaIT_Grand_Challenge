#pragma once

#include "Lane.hpp"
#include "TrafficSign.hpp"

#include <vector>

using LaneList = std::vector<Lane>;
using TrafficSignList = std::vector<TrafficSign>;

class Road {
public:
    explicit Road(LaneList&& lanes, TrafficSignList&& signs)
        : mLanes(std::move(lanes))
        , mSigns(std::move(signs))
    {
    }

    const LaneList& getLanes() const { return mLanes; }
    const TrafficSignList getSigns() const { return mSigns; }

private:
    LaneList mLanes;
    TrafficSignList mSigns;
};
