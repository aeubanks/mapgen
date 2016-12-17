//
// Created by Arthur Eubanks on 7/1/16.
//

#include "lodepng_map.hpp"

#include "lodepng.hpp"

namespace lodepng {
std::vector<unsigned char> map_to_image(mapgen::Map & map, int tile_size_i) {
    assert(tile_size_i > 0);

    unsigned width = static_cast<unsigned>(map.width());
    unsigned height = static_cast<unsigned>(map.height());
    unsigned tile_size = static_cast<unsigned>(tile_size_i);
    std::vector<unsigned char> image(width * height * tile_size * tile_size * 4,
                                     0xFF);
    for (auto && pair : map.coords_values()) {
        unsigned char val = (pair.value.type == mapgen::MapTileType::Ground)
                                ? static_cast<unsigned char>(0xFF)
                                : static_cast<unsigned char>(0x00);
        for (unsigned j = 0; j < tile_size; ++j) {
            auto y = (pair.coord.y * tile_size + j) * width * tile_size;
            for (unsigned i = 0; i < tile_size; ++i) {
                auto x = pair.coord.x * tile_size + i;
                auto idx = (y + x) * 4;
                image[idx + 0] = val;
                image[idx + 1] = val;
                image[idx + 2] = val;
            }
        }
    }
    return image;
}

std::vector<unsigned char> map_to_image(mapgen::Map && map, int tile_size) {
    return map_to_image(map, tile_size);
}

void save_map_as_image(std::string file_name, mapgen::Map & map, int tile_size) {
    auto image = map_to_image(map, tile_size);
    auto err = lodepng::encode(file_name, image,
                               static_cast<unsigned>(map.width() * tile_size),
                               static_cast<unsigned>(map.height() * tile_size));
    fmt::println_obj(err);
    fmt::println_obj(lodepng_error_text(err));
}

//	void save_map_as_image(mapgen::Map & map, std::string file_name) {
//		save_map_as_image(map, 1, file_name);
//	}
}
