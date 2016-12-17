//
// Created by I7 on 5/16/2016.
//

#include "Map2.hpp"
#include "mg_util/Random.hpp"

namespace game {
Map2::Map2(int width, int height) : tile_types_(width, height) {
    mg_util::Random rand;
    for (auto coord : coords()) {
        tile_types_[coord] = rand.rand_int_exc(MapTileType2::num_types());
    }
}

int Map2::width() { return tile_types_.width(); }

int Map2::height() { return tile_types_.height(); }

auto Map2::coords() -> decltype(tile_types_.coords()) {
    return tile_types_.coords();
}

auto Map2::coords() const -> decltype(tile_types_.coords()) {
    return tile_types_.coords();
}

const MapTileType2 & Map2::operator[](mg_util::Coord2D coord) {
    auto id = tile_types_[coord];
    return MapTileType2::get_type(id);
}
}
