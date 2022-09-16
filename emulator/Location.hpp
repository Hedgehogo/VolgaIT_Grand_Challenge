#pragma once

#include "Unit.hpp"

#include <cmath>

struct Location {
    LengthUnit x;
    LengthUnit y;

    bool isInRange(Location loc, LengthUnit range) const
    {
        const auto dx = x - loc.x;
        const auto dy = y - loc.y;
        return dx * dx + dy * dy <= range * range;
    }

    Location moved(Direction dir, LengthUnit speed) const
    {
        return { x + dir.dx * speed, y + dir.dy * speed };
    }
};
