#pragma once

#include "mapgen_include.hpp"

namespace mapgen {

enum class MapTileType {
    None,
    Connection,
    Wall,
    Ground,
    Grass,
    Bush,
    Forest,
    Mountain,
    Door,
};

// the name of a tile type
std::string MapTileType_to_string(MapTileType tile);

// the char representing a tile type
char MapTileType_to_char(MapTileType tile);

MapTileType char_to_MapTileType(char c);
}

