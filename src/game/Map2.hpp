#pragma once

#include "MapTileType2.hpp"
#include "mg_util/Array2D.hpp"
#include "mg_util/Coord2D.hpp"

namespace game {
class Map2 {
  private:
    mg_util::Array2D<int> tile_types_;

  public:
    Map2(int width, int height);
    const MapTileType2 & operator[](mg_util::Coord2D coord);
    int width();
    int height();
    auto coords() -> decltype(tile_types_.coords());
    auto coords() const -> decltype(tile_types_.coords());
};
}

