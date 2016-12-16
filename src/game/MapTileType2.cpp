//
// Created by I7 on 5/16/2016.
//

#include "MapTileType2.hpp"
#include <fstream>
#include "mg_util/text_processing.hpp"

namespace game {
    int MapTileType2::cur_id;
    std::vector <MapTileType2> MapTileType2::types;

    const MapTileType2& MapTileType2::get_type(int id) {
        return types.at(id);
    }

    int MapTileType2::num_types() {
        return types.size();
    }

    void MapTileType2::init() {
        std::ifstream file("resources/tiles.txt");
		std::string line;
        while (std::getline(file, line)) {
            auto split = mg_util::split(line, ' ');
            assert(split.size() == 3);
			std::string name = split[0];
            int x, y;
            if (!mg_util::str2int(split[1], &x)) {
				throw mg_util::mg_error(fmt::format("invalid x number: {}", split[1]));
            }
            if (!mg_util::str2int(split[2], &y)) {
                throw mg_util::mg_error(fmt::format("invalid y number: {}", split[2]));
            }
            types.emplace_back(cur_id++, name, x, y);
        }
    }

    MapTileType2::MapTileType2(int id, std::string &name, int x_tile, int y_tile) : id_(id), name_(name), x_tile_(x_tile),
                                                                               y_tile_(y_tile) { }

    int MapTileType2::id() const {
        return id_;
    }

    const std::string & MapTileType2::name() const  {
        return name_;
    }

    int MapTileType2::x_tile() const {
        return x_tile_;
    }

    int MapTileType2::y_tile() const {
        return y_tile_;
    }
}