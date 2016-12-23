#ifndef MAPPNG_HPP
#define MAPPNG_HPP

#include <string>
#include "mapgen/Map.hpp"

namespace mappng {
void map_to_png(const std::string & file_name, const mapgen::Map & map, int tile_size = 1);
}

#endif
