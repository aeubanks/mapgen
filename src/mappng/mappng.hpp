#pragma once

#include <string>
#include "mapgen/Map.hpp"

namespace mappng {
void map_to_png(const std::string & file_name, const mapgen::Map & map, int32_t tile_size = 1);
}

