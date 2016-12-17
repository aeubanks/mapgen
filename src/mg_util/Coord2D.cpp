//
// Created by Arthur Eubanks on 20/12/15.
//

#include "Coord2D.hpp"

namespace mg_util {
Coord2D operator+(const Coord2D & a, const Coord2D & b) {
    Coord2D c = a;
    return c += b;
}

Coord2D operator-(const Coord2D & a, const Coord2D & b) {
    Coord2D c = a;
    return c -= b;
}

Coord2D operator*(const Coord2D & c, int i) { return Coord2D(c.x * i, c.y * i); }

Coord2D operator*(int i, const Coord2D & c) { return c * i; }

Coord2D operator/(const Coord2D & c, int i) { return Coord2D(c.x / i, c.y / i); }

bool operator==(Coord2D a, Coord2D b) { return a.x == b.x && a.y == b.y; }

bool operator!=(Coord2D a, Coord2D b) { return !operator==(a, b); }

std::ostream & operator<<(std::ostream & os, const Coord2D & coord) {
    return os << "(" << coord.x << ", " << coord.y << ")";
}
}