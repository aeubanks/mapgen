#pragma once

#include "mg_util/mg_util_include.hpp"

#include <vector>
#include <tuple>

#include "mg_util/Random.hpp"

namespace mg_util {

template<typename T>
class WeightedSet {
public:
    using ValueType = T;
    using WeightType = int32_t;

    WeightedSet(): items() {}

    auto size() const {
        return items.size();
    }

    auto totalWeight() const {
        return size() == 0 ? 0 : std::get<1>(items.back());
    }

    void add(WeightType weight, T item) {
        if (weight <= 0) {
            throw mg_error("invalid weight added to WeightedSet");
        }
        items.push_back(item, weight + totalWeight());
    }

    template<typename... Es>
    void emplace(WeightType weight, Es... emplaces) {
        if (weight <= 0) {
            throw mg_error("invalid weight added to WeightedSet");
        }
        items.emplace_back(std::forward<Es>(emplaces)..., weight + totalWeight());
    }

    const T & random(Random & rand) const {
        auto size = this->size();
        if (size == 1) {
            return std::get<0>(items[0]);
        }

        // binary search on random weight
        WeightType randCdf = rand.rand_int_exc(size);
        size_t lo = 0, hi = size - 1;
        while (lo != hi) {
            auto cur = lo + (hi - lo) / 2;
            auto curCdf = std::get<1>(items[cur]);
            if (randCdf < curCdf) {
                hi = cur;
            } else {
                lo = cur;
            }
        }
        return std::get<0>(items[lo]);
    }

    T & random(Random & rand) {
        auto size = this->size();
        if (size == 1) {
            return std::get<0>(items[0]);
        }

        // binary search on random weight
        WeightType randCdf = rand.rand_int_exc(size);
        size_t lo = 0, hi = size - 1;
        while (lo != hi) {
            auto cur = lo + (hi - lo) / 2;
            auto curCdf = std::get<1>(items[cur]);
            if (randCdf < curCdf) {
                hi = cur;
            } else {
                lo = cur;
            }
        }
        return std::get<0>(items[lo]);
    }

private:
    std::vector<std::tuple<T, WeightType>> items;
};

}

