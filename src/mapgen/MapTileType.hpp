//
// Created by Arthur Eubanks on 20/12/15.
//

#ifndef MAP_GEN_MAPTILETYPE_H
#define MAP_GEN_MAPTILETYPE_H

#include "mapgen_global.hpp"

namespace mapgen {
// an enum for each of the possible tile types
enum class MapTileType { Wall,
                         Ground,
                         Grass,
                         Bush,
                         Forest,
                         Mountain };

// the name of a tile type
string MapTileType_to_string(MapTileType tile);

// the char representing a tile type
char MapTileType_to_char(MapTileType tile);
}

#endif // MAP_GEN_MAPTILETYPE_H
