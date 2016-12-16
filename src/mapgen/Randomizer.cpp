//
// Created by Arthur Eubanks on 28/12/15.
//

#include "Randomizer.hpp"

namespace mapgen {
	void Randomizer::modify_map(Map & map) {
		auto randomizer = [this](decltype(*map.begin()) tile) {
			tile.type = rand_.rand_bool(chance_) ? MapTileType::Ground : MapTileType::Wall;
		};
		std::for_each(map.begin(), map.end(), randomizer);
	}
}