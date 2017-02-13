#pragma once

#include "mg_util_include.hpp"

#include "Coord2D.hpp"

namespace mg_util {
class Direction {
private:
    enum class DirectionEnum {
        Up, Down, Left, Right
    };

    DirectionEnum dir;

public:
    static Direction Up;
    static Direction Down;
    static Direction Left;
    static Direction Right;

    Direction(DirectionEnum dir): dir(dir) {}

    Direction cw();
    Direction ccw();
    Coord2D coord();
};
}

mg_util::Coord2D & operator+=(mg_util::Coord2D & coord, mg_util::Direction dir);
mg_util::Coord2D operator+(mg_util::Coord2D coord, mg_util::Direction dir);
mg_util::Coord2D operator+(mg_util::Direction dir, mg_util::Coord2D coord);

