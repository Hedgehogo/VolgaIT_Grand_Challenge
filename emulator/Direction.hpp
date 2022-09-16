#pragma once

#include "Unit.hpp"

#include <cmath>

struct Direction {
    LengthUnit dx;
    LengthUnit dy;

    LengthUnit product(Direction other) const //
    {
        return dx * other.dx + dy * other.dy;
    }

    LengthUnit length() const //
    {
        return std::sqrt(dx * dx + dy * dy);
    }

    Direction normalized() const
    {
        const auto len = length();
        return { dx / len, dy / len };
    }

    Direction rotated(AngleUnit angle) const
    {
        const auto sin = std::sin(angle);
        const auto cos = std::cos(angle);
        return { dx * cos - dy * sin, dx * sin + dy * cos };
    }
};
