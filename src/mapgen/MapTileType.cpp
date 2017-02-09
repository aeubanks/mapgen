//
// Created by Arthur Eubanks on 20/12/15.
//

#include "MapTileType.hpp"

#include <fstream>

#include "mg_util/text_processing.hpp"

namespace mapgen {
static string map_tile_names[] = {
    "None",
    "Connection",
    "Wall",
    "Ground",
    "Grass",
    "Bush",
    "Forest",
    "Mountain",
    "Door",
};

string MapTileType_to_string(MapTileType tile) {
    return map_tile_names[static_cast<int>(tile)];
}

char MapTileType_to_char(MapTileType tile) {
    switch (tile) {
        case MapTileType::None:
            return '~';
        case MapTileType::Connection:
            return '?';
        case MapTileType::Wall:
            return '#';
        case MapTileType::Ground:
            return '.';
        case MapTileType::Grass:
            return ',';
        case MapTileType::Bush:
            return ';';
        case MapTileType::Forest:
            return '"';
        case MapTileType::Mountain:
            return '^';
        case MapTileType::Door:
            return '+';
        default:
            mg_log::error("unexpected tile for MapTileType_to_char: ", MapTileType_to_string(tile));
            return ' ';
    }
}

MapTileType char_to_MapTileType(char c) {
    switch (c) {
        case '~':
            return MapTileType::None;
        case '?':
            return MapTileType::Connection;
        case '#':
            return MapTileType::Wall;
        case '.':
            return MapTileType::Ground;
        case ',':
            return MapTileType::Grass;
        case ';':
            return MapTileType::Bush;
        case '"':
            return MapTileType::Forest;;
        case '^':
            return MapTileType::Mountain;
        case '+':
            return MapTileType::Door;
        default:
            mg_log::error("unexpected char for char_to_MapTileType: ", c);
            return MapTileType::None;
    }
}

}
