#ifndef MAP_GEN_COORD2D_H
#define MAP_GEN_COORD2D_H

#include "mg_util_global.hpp"
#include <ostream>

namespace mg_util {
// a class to index Array2D
// has an x and y
class Coord2D {
  public:
    using DimType = int32_t;
    DimType x, y;

    Coord2D(int x_, int y_) : x(x_), y(y_) {}

    // offset the x and y by dx and dy
    void offset(int dx, int dy) {
        x += dx;
        y += dy;
    }

    // return a coord that is this one + (dx, dy)
    Coord2D offseted(int dx, int dy) const {
        Coord2D ret = *this;
        ret.offset(dx, dy);
        return ret;
    }

    // return if x and y are >= 0 and < width/height
    // basically, can this coord be used to index an Array2D with the specified
    // width/height
    bool in_bounds(int width, int height) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    // basic arithmetic operations
    Coord2D & operator+=(Coord2D other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Coord2D & operator-=(Coord2D other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

// basic arithmetic operations
Coord2D operator+(const Coord2D & a, const Coord2D & b);
Coord2D operator-(const Coord2D & a, const Coord2D & b);

Coord2D operator*(const Coord2D & c, int i);
Coord2D operator*(int i, const Coord2D & c);
Coord2D operator/(const Coord2D & c, int i);

// equality
bool operator==(Coord2D a, Coord2D b);
bool operator!=(Coord2D a, Coord2D b);

std::ostream & operator<<(std::ostream & os, const Coord2D & coord);
}

namespace std {
template <>
struct hash<mg_util::Coord2D> {
    // based on boost::hash_combine()
    // http://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
    std::size_t operator()(const mg_util::Coord2D & coord) const {
        std::size_t seed = 0;
        seed ^= static_cast<decltype(seed)>(coord.x) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
        seed ^= static_cast<decltype(seed)>(coord.y) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
        return seed;
    }
};
}

#endif // MAP_GEN_COORD2D_H
