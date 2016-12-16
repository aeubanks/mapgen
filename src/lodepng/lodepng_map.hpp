//
// Created by Arthur Eubanks on 7/1/16.
//

#ifndef MAPGEN_LODEPNG_MAP_HPP
#define MAPGEN_LODEPNG_MAP_HPP

#include <vector>
#include <string>

#include "mapgen/Map.hpp"

namespace lodepng {
	std::vector<unsigned char> map_to_image(mapgen::Map & map, int tile_size = 1);
	std::vector<unsigned char> map_to_image(mapgen::Map && map, int tile_size = 1);

	void save_map_as_image(std::string file_name, mapgen::Map & map, int tile_size = 1);
}


#endif //MAPGEN_LODEPNG_MAP_HPP
