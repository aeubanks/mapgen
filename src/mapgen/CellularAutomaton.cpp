//
// Created by Arthur Eubanks on 20/12/15.
//

#include "CellularAutomaton.hpp"

namespace mapgen {
bool CARuleAliveDeadLimit::operator()(Map & map, Coord2D coord) {
    int num_neighbors =
        map.num_neighbors_of_type_moore(MapTileType::Ground, coord);
    int limit =
        map[coord].type == MapTileType::Ground ? alive_limit_ : dead_limit_;
    return num_neighbors >= limit;
}
}
