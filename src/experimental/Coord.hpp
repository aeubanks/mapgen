#pragma once

#include <cstdint>
#include <ostream>

#include <boost/functional/hash.hpp>

namespace mg_util {

template<std::size_t Dims>
class Coord {
public:
    using DimType = std::int32_t;
    DimType vals[Dims];

    Coord(): vals() {}

    template<typename... Vals>
    Coord(Vals... vals): vals(vals...) {
        static_assert(sizeof...(vals) == Dims, "incorrect number of dimensions");
    }

    const DimType & operator[](decltype(Dims) d) const {
        return vals[d];
    }

    DimType & operator[](decltype(Dims) d) {
        return vals[d];
    }

    // offset the x and y by dx and dy
    template<typename... Vals>
    void offset(Vals... vals) {
        static_assert(sizeof...(vals) == Dims, "incorrect number of dimensions");
        *this = *this + Coord{vals...};
    }

    // return a coord that is this one + (dx, dy)
    template<typename... Vals>
    Coord offseted(Vals... vals) const {
        static_assert(sizeof...(vals) == Dims, "incorrect number of dimensions");
        Coord<Dims> ret = *this;
        ret.offset(vals...);
        return ret;
    }

    template<typename... BoundVals>
    bool in_bounds(BoundVals... boundVals) const {
        Coord bounds(boundVals...);
        for (std::int16_t i = 0; i < Dims; ++i) {
            if ((*this)[i] < 0 || (*this[i]) >= bounds[i]) {
                return false;
            }
        }
        return true;
    }

    // basic arithmetic operations
    Coord & operator+=(const Coord & other) {
        for (std::int16_t i = 0; i < Dims; ++i) {
            (*this)[i] += other[i];
        }
        return *this;
    }

    Coord & operator-=(const Coord & other) {
        for (std::int16_t i = 0; i < Dims; ++i) {
            (*this)[i] -= other[i];
        }
        return *this;
    }
};

// basic arithmetic operations
template<std::uint8_t Dims>
Coord<Dims> operator+(const Coord<Dims> & a, const Coord<Dims> & b) {
    Coord<Dims> ret = a;
    ret += b;
    return ret;
}

template<std::uint8_t Dims>
Coord<Dims> operator-(const Coord<Dims> & a, const Coord<Dims> & b) {
    Coord<Dims> ret = a;
    ret -= b;
    return ret;
}

template<std::uint8_t Dims>
Coord<Dims> operator*(const Coord<Dims> & c, typename Coord<Dims>::DimType i) {
    Coord<Dims> ret = c;
    for (std::int16_t d = 0; d < Dims; ++d) {
        ret[d] *= i;
    }
    return ret;
}

template<std::uint8_t Dims>
Coord<Dims> operator*(typename Coord<Dims>::DimType i, const Coord<Dims> & c) {
    return c * i;
}

template<std::uint8_t Dims>
Coord<Dims> operator/(const Coord<Dims> & c, typename Coord<Dims>::DimType i) {
    Coord<Dims> ret = c;
    for (std::int16_t d = 0; d < Dims; ++d) {
        ret[d] /= i;
    }
    return ret;
}

// equality
template<std::uint8_t Dims>
bool operator==(Coord<Dims> a, Coord<Dims> b) {
    for (std::int16_t i = 0; i < Dims; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

template<std::uint8_t Dims>
bool operator!=(Coord<Dims> a, Coord<Dims> b) {
    for (std::int16_t i = 0; i < Dims; ++i) {
        if (a[i] == b[i]) {
            return true;
        }
    }
    return false;
}

template<std::uint8_t Dims>
std::ostream & operator<<(std::ostream & os, const Coord<Dims> & coord);

using Coord2D = Coord<2>;
}

namespace std {
template<std::uint8_t Dims>
struct hash<mg_util::Coord<Dims>> {
    // based on boost::hash_combine()
    // http://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
    std::size_t operator()(const mg_util::Coord<Dims> & coord) const {
        std::size_t seed = 0;
        for (std::int16_t i = 0; i < Dims; ++i) {
            boost::hash_combine(seed, coord[i]);
        }
        return seed;
    }
};
}

