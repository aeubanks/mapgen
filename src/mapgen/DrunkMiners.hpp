//
// Created by Arthur Eubanks on 31/12/15.
//

#ifndef MAPGEN_DRUNKMINERS_HPP
#define MAPGEN_DRUNKMINERS_HPP


#include "MapGenerator.hpp"

namespace mapgen {
	class DrunkMiners: public MapGenerator {
	public:
		DrunkMiners(mg_util::Random & r, double spawn_prob, int neighbor_ground_limit): MapGenerator(r), spawn_prob_(spawn_prob), neighbor_ground_limit_(neighbor_ground_limit) {}

		void modify_map(Map & map) override;

	private:
		double spawn_prob_;
		int neighbor_ground_limit_;
	};
}


#endif //MAPGEN_DRUNKMINERS_HPP
