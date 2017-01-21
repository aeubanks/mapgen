//
// Created by Arthur Eubanks on 20/12/15.
//

#include "MapTileType.hpp"

#include <fstream>

#include "mg_util/text_processing.hpp"

namespace mapgen {
static string map_tile_names[] = {
    "Wall", "Ground",
};

string MapTileType_to_string(MapTileType tile) {
    return map_tile_names[static_cast<int>(tile)];
}

char MapTileType_to_char(MapTileType tile) {
    switch (tile) {
        case MapTileType::Wall:
            return '#';
        case MapTileType::Ground:
            return '.';
        default:
            mg_log::error("unexpected tile for MapTileType_to_char: ", MapTileType_to_string(tile));
            return ' ';
    }
}
}
