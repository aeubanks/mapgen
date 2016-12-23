//
// Created by Arthur Eubanks on 7/1/16.
//

#ifndef MAPBYTES_HPP
#define MAPBYTES_HPP

#include <vector>

#include "mapgen/Map.hpp"

namespace mapbytes {
std::vector<unsigned char> map_to_bytes(const mapgen::Map & map, int tile_size = 1);
std::vector<unsigned char> map_to_bytes(mapgen::Map && map, int tile_size = 1);
}

#endif // MAPBYTES_HPP
