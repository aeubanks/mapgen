//
// Created by Arthur Eubanks on 31/12/15.
//

#include "DrunkMiners.hpp"

namespace mapgen {
void DrunkMiners::modify_map(Map & map) {
    bool is_large_wall;
    do {
        vector<Coord2D> miners{{rand_.rand_int_exc(0, map.width()),
                                rand_.rand_int_exc(0, map.height())}};
        while (!miners.empty()) {
            for (int i = static_cast<decltype(i)>(miners.size() - 1); i >= 0; --i) {
                auto miner = miners[i];
                // duplicate miner
                if (rand_.rand_bool(spawn_prob_)) {
                    miners.push_back(miner);
                }
                int num_ground_neighbors =
                    8 - map.num_neighbors_of_type_moore(MapTileType::Wall, miner);

                if (num_ground_neighbors >= neighbor_ground_limit_) {
                    // delete current miner
                    miners[i] = miners.back();
                    miners.pop_back();
                } else {
                    Coord2D rand_dir(rand_.rand_int_inc(-1, 1),
                                     rand_.rand_int_inc(-1, 1));
                    auto new_miner = miner + rand_dir;
                    if (map.in_bounds(new_miner)) {
                        miners[i] = new_miner;
                        map[new_miner] = MapTileType::Ground;
                    }
                }
            }
        }
        is_large_wall = false;
        auto lambda = [&is_large_wall](Map & map2, vector<Coord2D> & section) {
            auto map_size =
                static_cast<decltype(section.size())>(map2.width() * map2.height());
            // if there is a wall section greater than 1/2 of the map, retry
            if (section.size() > map_size / 2) {
                is_large_wall = true;
            }
        };
        map.sections_for_each_von_neumann(MapTileType::Wall, lambda);
    } while (is_large_wall);
}
}
