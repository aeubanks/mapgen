//
// Created by Arthur Eubanks on 20/12/15.
//

#ifndef MAP_GEN_MAPTILE_H
#define MAP_GEN_MAPTILE_H

#include "mapgen_global.hpp"
#include "MapTileType.hpp"

namespace mapgen {
	// a class representing a tile on a map
	class MapTile {
	public:
		MapTile(MapTileType t) : type(t) { }

		MapTileType type;
	};
}


#endif //MAP_GEN_MAPTILE_H
