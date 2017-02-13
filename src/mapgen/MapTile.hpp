#pragma once

#include "mapgen_include.hpp"

#include "MapTileType.hpp"

namespace mapgen {
// a class representing a tile on a map
class MapTile {
  public:
    MapTile(MapTileType t) : type(t) {}

    MapTileType type;
};
}

