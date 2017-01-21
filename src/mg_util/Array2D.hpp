//
// Created by Arthur Eubanks on 24/12/15.
//

#ifndef MAPGEN_ARRAY2D_H
#define MAPGEN_ARRAY2D_H

#include <algorithm>
#include <functional>
#include <ostream>

#include "Coord2D.hpp"
#include "mg_util_global.hpp"
#include "mg_log/Log.hpp"

namespace mg_util {
template <typename T>
class Array2D {
    // an iterator for an Array2D that provides all of the coords
    class Array2DCoordIterator_ {
      public:
        Array2DCoordIterator_(Array2DCoordIterator_ & other)
            : array_(other.array_), cur_coord_(other.cur_coord_) {}

        Array2DCoordIterator_(Array2DCoordIterator_ && other)
            : array_(other.array_), cur_coord_(other.cur_coord_) {}

        Array2DCoordIterator_(const Array2D<T> & arr, Coord2D coord)
            : array_(arr), cur_coord_(coord) {}

        bool operator==(Array2DCoordIterator_ other) const {
            return cur_coord_ == other.cur_coord_;
        }

        bool operator!=(Array2DCoordIterator_ other) const {
            return !operator==(other);
        }

        Array2DCoordIterator_ & operator++() {
            ++cur_coord_.x;
            if (cur_coord_.x == array_.width()) {
                cur_coord_.x = 0;
                ++cur_coord_.y;
            }
            return *this;
        }

        Coord2D operator*() const { return cur_coord_; }

      private:
        const Array2D<T> & array_;
        Coord2D cur_coord_;
    };

    // an iterable for Array2D that goes through all coords
    // returned from Array2d::coords
    class Array2DCoordIterable_ {
      public:
        Array2DCoordIterable_(const Array2D<T> & arr) : array(arr) {}

        Array2DCoordIterator_ begin() {
            return Array2DCoordIterator_(array, Coord2D(0, 0));
        }

        Array2DCoordIterator_ end() {
            return Array2DCoordIterator_(array, Coord2D(0, array.height()));
        }

      private:
        const Array2D<T> & array;
    };

    // a struct that holds a coord and the corresponding value of an Array2D
    // ValueType is meant to be either 'T' or 'const T' depending on the
    // application
    // returned from Array2DCoordValueIteratorTemplate_
    template <typename ValueType>
    class Array2DCoordValuePairTemplate_ {
      public:
        Coord2D coord;
        ValueType & value;

        Array2DCoordValuePairTemplate_(Coord2D c, ValueType & val)
            : coord(c), value(val) {}
    };

    using Array2DCoordValuePair_ = Array2DCoordValuePairTemplate_<T>;
    using Array2DCoordValuePairConst_ = Array2DCoordValuePairTemplate_<const T>;

    // an iterator for an Array2D that provides all of the coords and values
    // through Array2DCoordValuePairTemplate_
    template <typename ArrayType, typename ReturnType>
    class Array2DCoordValueIteratorTemplate_ {
      public:
        using ThisType = Array2DCoordValueIteratorTemplate_<ArrayType, ReturnType>;

        Array2DCoordValueIteratorTemplate_(ArrayType & arr, Coord2D coord)
            : array_(arr), cur_coord_(coord) {}

        bool operator==(ThisType other) const {
            return cur_coord_ == other.cur_coord_;
        }

        bool operator!=(ThisType other) const { return !operator==(other); }

        auto operator++() -> ThisType & {
            ++cur_coord_.x;
            if (cur_coord_.x == array_.width()) {
                cur_coord_.x = 0;
                ++cur_coord_.y;
            }
            return *this;
        }

        ReturnType operator*() const {
            return ReturnType(cur_coord_, array_[cur_coord_]);
        }

      private:
        ArrayType & array_;
        Coord2D cur_coord_;
    };

    using Array2DCoordValueIterator_ =
        Array2DCoordValueIteratorTemplate_<Array2D<T>, Array2DCoordValuePair_>;
    using Array2DCoordValueIteratorConst_ =
        Array2DCoordValueIteratorTemplate_<const Array2D<T>,
                                           Array2DCoordValuePairConst_>;

    // an iterable for Array2D that goes through all coords
    // returned from Array2d::coords_values
    template <typename ArrayType, typename ReturnType>
    class Array2DCoordValueIterableTemplate_ {
      public:
        Array2DCoordValueIterableTemplate_(ArrayType & arr) : array_(arr) {}

        using IteratorType = Array2DCoordValueIteratorTemplate_<
            ArrayType, Array2DCoordValuePairTemplate_<ReturnType>>;

        IteratorType begin() { return IteratorType(array_, {0, 0}); }

        IteratorType end() { return IteratorType(array_, {0, array_.height()}); }

      private:
        ArrayType & array_;
    };

    using Array2DCoordValueIterable_ =
        Array2DCoordValueIterableTemplate_<Array2D<T>, T>;
    using Array2DCoordValueIterableConst_ =
        Array2DCoordValueIterableTemplate_<const Array2D<T>, const T>;

  private:
    int32_t width_, height_;
    vector<T> values_;

  public:
    Array2D(int32_t width, int32_t height, T init_val = T())
        : width_(width), height_(height),
          values_(static_cast<unsigned long>(width * height), init_val) {}

    auto width() const { return width_; }
    auto height() const { return height_; }

    // return if the coord is a valid coord to index the Array2D
    bool in_bounds(Coord2D coord) const {
        return coord.in_bounds(width_, height_);
    }

    // return a reference to the requested value at coord
    typename decltype(values_)::reference operator[](Coord2D coord) {
        if constexpr (DEBUG) {
            if (!in_bounds(coord)) {
                mg_log::error("out of bounds: accessing ", coord, " (bounds ", width_, ", ", height_, ")");
                throw mg_error("");
            }
        }
        return values_[coord.x + coord.y * width_];
    }

    typename decltype(values_)::const_reference operator[](Coord2D coord) const {
        if constexpr (DEBUG) {
            if (!in_bounds(coord)) {
                mg_log::error("out of bounds: accessing ", coord, " (bounds ", width_, ", ", height_, ")");
                throw mg_error("");
            }
        }
        return values_[coord.x + coord.y * width_];
    }

    // calls f(value) on each value
    template <typename FuncType>
    void for_each(FuncType f) {
        std::for_each(values_.begin(), values_.end(), f);
    }

    // iterators
    decltype(values_.begin()) begin() { return values_.begin(); }
    decltype(values_.end()) end() { return values_.end(); }
    decltype(values_.cbegin()) begin() const { return values_.cbegin(); }
    decltype(values_.cend()) end() const { return values_.cend(); }
    decltype(values_.cbegin()) cbegin() const { return values_.cbegin(); }
    decltype(values_.cend()) cend() const { return values_.cend(); }

    // return an iterable of all valid coords
    Array2DCoordIterable_ coords() const { return Array2DCoordIterable_(*this); }

    // return an iterable of all valid coords and the corresponding value
    Array2DCoordValueIterable_ coords_values() {
        return Array2DCoordValueIterable_(*this);
    }
    Array2DCoordValueIterableConst_ coords_values() const {
        return Array2DCoordValueIterableConst_(*this);
    }
};

template <typename T>
std::ostream & operator<<(std::ostream & os, const Array2D<T> & arr) {
    for (int32_t y = 0; y < arr.height(); y++) {
        for (int32_t x = 0; x < arr.width(); x++) {
            os << arr[{x, y}];
            if (x != arr.width() - 1) {
                os << ' ';
            }
        }
        if (y != arr.height() - 1) {
            os << '\n';
        }
    }
    return os;
}
}

#endif // MAPGEN_ARRAY2D_H
