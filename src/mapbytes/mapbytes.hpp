#pragma once

#include "mapbytes_include.hpp"

#include <vector>

#include "mapgen/Map.hpp"

namespace mapbytes {
std::vector<unsigned char> map_to_bytes(const mapgen::Map & map, int32_t tile_size = 1);
}

