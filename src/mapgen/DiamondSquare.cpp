//
// Created by Arthur Eubanks on 24/12/15.
//

#include "DiamondSquare.hpp"

#include "mg_log/Log.hpp"
#include "mg_util/mg_math.hpp"

namespace mapgen {
DiamondSquare::DiamondSquare(mg_util::Random & r, double smoothness)
    : MapGenerator(r), smoothness_(smoothness) {}

void DiamondSquare::modify_map(Map & map) {
    if constexpr (DEBUG) {
        if (smoothness_ <= 0.0) {
            mg_log::error("invalid smoothness");
            throw mg_util::mg_error("");
        }
    }

    double deviation = 1.0;
    int width = map.width();
    int height = map.height();
    int inc =
        std::min(mg_util::pow2rounddown(width), mg_util::pow2rounddown(height));

    Array2D<double> values(width, height);

    init_vals(inc, deviation / 4.0, values);

    inc >>= 1;

    while (inc > 0) {
        divide(inc, deviation, values, map);
        inc >>= 1;
        deviation /= smoothness_;
    }

    scale_values(values);

    values_to_map(0.5, values, map);
}

void DiamondSquare::init_vals(int inc, double deviation,
                              Array2D<double> & values) {
    for (int y = 0; y < values.height(); y += inc) {
        for (int x = 0; x < values.width(); x += inc) {
            values[{x, y}] = rand_.rand_double(-deviation, deviation);
        }
    }
}

void DiamondSquare::divide(int inc, double deviation, Array2D<double> & values,
                           Map & map) {
    for (int y = inc; y < values.height(); y += inc * 2) {
        for (int x = inc; x < values.width(); x += inc * 2) {
            square(x, y, inc, deviation, values, map);
        }
    }

    for (int y = 0; y < map.height(); y += inc * 2) {
        for (int x = inc; x < map.width(); x += inc * 2) {
            diamond(x, y, inc, deviation, values, map);
        }
    }

    for (int y = inc; y < map.height(); y += inc * 2) {
        for (int x = 0; x < map.width(); x += inc * 2) {
            diamond(x, y, inc, deviation, values, map);
        }
    }
}

void DiamondSquare::square(int x, int y, int inc, double deviation,
                           Array2D<double> & values, Map & map) {
    double rand_val = rand_.rand_double(-deviation, deviation);
    double average = average_of_valid_coords(
        {x + inc, y + inc}, {x - inc, y - inc}, {x - inc, y + inc},
        {x + inc, y - inc}, values, map);
    values[{x, y}] = rand_val + average;
}

void DiamondSquare::diamond(int x, int y, int inc, double deviation,
                            Array2D<double> & values, Map & map) {
    double rand_val = rand_.rand_double(-deviation, deviation);
    double average = average_of_valid_coords(
        {x + inc, y}, {x - inc, y}, {x, y + inc}, {x, y - inc}, values, map);
    values[{x, y}] = rand_val + average;
}

double DiamondSquare::average_of_valid_coords(Coord2D a, Coord2D b, Coord2D c,
                                              Coord2D d,
                                              Array2D<double> & values,
                                              Map & map) {
    double total = 0.0, count = 0.0;
    if (map.in_bounds(a)) {
        total += values[a];
        count += 1.0;
    }
    if (map.in_bounds(b)) {
        total += values[b];
        count += 1.0;
    }
    if (map.in_bounds(c)) {
        total += values[c];
        count += 1.0;
    }
    if (map.in_bounds(d)) {
        total += values[d];
        count += 1.0;
    }
    return total / count;
}

void DiamondSquare::scale_values(Array2D<double> & values) {
    // find min and max
    double min, max;
    min = max = values[{0, 0}];
    values.for_each([&min, &max](double & val) {
        if (val < min) {
            min = val;
        }
        if (val > max) {
            max = val;
        }
    });
    double dif = max - min;
    if (dif == 0.0) { // if min == max then just set everything to 0
        mg_log::info("DiamondSquare::scale_values() has min == max");
        values.for_each([min, dif](double & val) { val = 0.0; });
    } else { // else scale all values to fit between 0.0 - 1.0
        double scale = 1.0 / dif;
        values.for_each([min, scale](double & val) { val = (val - min) * scale; });
    }
}

void DiamondSquare::values_to_map(double threshold, Array2D<double> & values,
                                  Map & map) {
    for (int y = 0; y < values.height(); ++y) {
        for (int x = 0; x < values.width(); ++x) {
            double val = values[{x, y}];
            bool above_thres = val >= threshold;
            MapTileType type = above_thres ? MapTileType::Ground : MapTileType::Wall;
            map[{x, y}] = type;
        }
    }
}
}
