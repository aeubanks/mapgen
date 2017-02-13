#pragma once

#include "mg_util_include.hpp"

#include <algorithm>
#include <functional>
#include <ostream>
#include <vector>

#include "Coord2D.hpp"
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

    // an iterable for Array2D that goes through all coords
    // returned from Array2d::coords_values
    template <typename ArrayType, typename ReturnType>
    class Array2DCoordValueIterableTemplate_ {
      public:
        Array2DCoordValueIterableTemplate_(ArrayType & arr) : array_(arr) {}

        using IteratorType = Array2DCoordValueIteratorTemplate_<ArrayType, Array2DCoordValuePairTemplate_<ReturnType>>;

        IteratorType begin() { return IteratorType(array_, {0, 0}); }

        IteratorType end() { return IteratorType(array_, {0, array_.height()}); }

      private:
        ArrayType & array_;
    };

    using Array2DCoordValueIterable_ = Array2DCoordValueIterableTemplate_<Array2D<T>, T>;
    using Array2DCoordValueIterableConst_ = Array2DCoordValueIterableTemplate_<const Array2D<T>, const T>;

    // a struct that holds a coord (private) and the corresponding value of an Array2D
    // ValueType is meant to be either 'T' or 'const T' depending on the
    // application
    // returned from Array2DValueIteratorTemplate_
    template <typename ValueType>
    class Array2DValuePairTemplate_ {
      public:
        Coord2D coord;
        ValueType & value;

        Array2DValuePairTemplate_(Coord2D c, ValueType & val) : coord(c), value(val) {}
    };

    using Array2DValuePair_ = Array2DValuePairTemplate_<T>;
    using Array2DValuePairConst_ = Array2DValuePairTemplate_<const T>;

    // an iterator for an Array2D that provides all of the coords and values
    // through Array2DValuePairTemplate_
    template <typename ArrayType, typename ReturnType>
    class Array2DValueIteratorTemplate_ {
      public:
        using ThisType = Array2DValueIteratorTemplate_<ArrayType, ReturnType>;

        Array2DValueIteratorTemplate_(ArrayType & arr, Coord2D coord) : array_(arr), cur_coord_(coord) {}

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
            return array_[cur_coord_];
        }

      private:
        ArrayType & array_;
        Coord2D cur_coord_;
    };

    using Array2DValueIterator_ = Array2DValueIteratorTemplate_<Array2D<T>, T &>;
    // returned from Array2d::values
    template <typename ArrayType, typename ReturnType>
    class Array2DValueIterableTemplate_ {
      public:
        Array2DValueIterableTemplate_(ArrayType & arr) : array_(arr) {}

        using IteratorType = Array2DValueIteratorTemplate_<ArrayType, ReturnType>;

        IteratorType begin() { return IteratorType(array_, {0, 0}); }

        IteratorType end() { return IteratorType(array_, {0, array_.height()}); }

      private:
        ArrayType & array_;
    };

    using Array2DValueIterable_ = Array2DValueIterableTemplate_<Array2D<T>, T &>;
    using Array2DValueIterableConst_ = Array2DValueIterableTemplate_<const Array2D<T>, const T &>;

  public:
    using SizeType = int32_t;

  private:
    SizeType width_, height_;
    std::vector<T> values_;

  public:
    Array2D(SizeType width, SizeType height, T init_val = T())
        : width_(width), height_(height),
          values_(width * height, init_val) {}

    auto width() const { return width_; }
    auto height() const { return height_; }

    // return if the coord is a valid coord to index the Array2D
    bool in_bounds(Coord2D coord) const {
        return coord.in_bounds(width_, height_);
    }

    // return a reference to the requested value at coord
    decltype(auto) operator[](Coord2D coord) {
        if constexpr (DEBUG) {
            if (!in_bounds(coord)) {
                mg_log::error("out of bounds: accessing ", coord, " (bounds ", width_, ", ", height_, ")");
                throw mg_error("");
            }
        }
        return values_[coord.x + coord.y * width_];
    }

    decltype(auto) operator[](Coord2D coord) const {
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
    auto begin() { return values_.begin(); }
    auto end() { return values_.end(); }
    auto begin() const { return values_.cbegin(); }
    auto end() const { return values_.cend(); }
    auto cbegin() const { return values_.cbegin(); }
    auto cend() const { return values_.cend(); }

    // return an iterable of all valid coords
    Array2DCoordIterable_ coords() const { return Array2DCoordIterable_(*this); }

    // return an iterable of all valid coords and the corresponding value
    Array2DCoordValueIterable_ coords_values() {
        return Array2DCoordValueIterable_(*this);
    }
    Array2DCoordValueIterableConst_ coords_values() const {
        return Array2DCoordValueIterableConst_(*this);
    }

    // return an iterable of all values
    Array2DValueIterable_ values() {
        return Array2DValueIterable_(*this);
    }
    Array2DValueIterableConst_ values() const {
        return Array2DValueIterableConst_(*this);
    }
};

template <typename T>
std::ostream & operator<<(std::ostream & os, const Array2D<T> & arr) {
    for (typename decltype(arr)::SizeType y = 0; y < arr.height(); y++) {
        for (typename decltype(arr)::SizeType x = 0; x < arr.width(); x++) {
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

