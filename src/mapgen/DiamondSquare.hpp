//
// Created by Arthur Eubanks on 24/12/15.
//

#ifndef MAPGEN_DIAMONDSQUARE_H
#define MAPGEN_DIAMONDSQUARE_H

#include "MapGenerator.hpp"

namespace mapgen {
class DiamondSquare : public MapGenerator {
  public:
    DiamondSquare(mg_util::Random & r, double smoothness);

    void modify_map(Map & map) override;

  private:
    // initializes the values, x and y incrementing by inc, with the deviation
    void init_vals(int inc, double deviation, Array2D<double> & values);
    // call square and diamond on the correct coords using inc
    void divide(int inc, double deviation, Array2D<double> & values, Map & map);
    // averages the square with length 2 * inc with x, y as the center's corner
    // values, adds a random deviation, and sets map[x, y]
    void square(int x, int y, int inc, double deviation, Array2D<double> & values,
                Map & map);
    // averages the diamond with length 2 * inc with x, y as the center's corner
    // values, adds a random deviation, and sets map[x, y]
    void diamond(int x, int y, int inc, double deviation, Array2D<double> & values,
                 Map & map);
    // returns the average of the 4 coords, ignoring any out of bounds coords
    double average_of_valid_coords(Coord2D a, Coord2D b, Coord2D c, Coord2D d,
                                   Array2D<double> & values, Map & map);
    // scales all values to between 0.0-1.0
    void scale_values(Array2D<double> & values);
    // for each coord, if the corresponding value is above the threshold, the tile
    // is ground, otherwise wall
    void values_to_map(double threshold, Array2D<double> & values, Map & map);

    // how much the deviation is divided by after each iteration
    double smoothness_;
};
}

#endif // MAPGEN_DIAMONDSQUARE_H
