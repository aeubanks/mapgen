#include "mapbytes.hpp"

#include "mg_log/Log.hpp"

namespace mapbytes {

std::vector<unsigned char> map_to_bytes(const mapgen::Map & map, int32_t tile_size) {
    if (DEBUG) {
        if (tile_size <= 0) {
            mg_log::error("invalid tile_size to map_to_bytes()");
            throw mg_util::mg_error("");
        }
    }

    auto width = map.width();
    auto height = map.height();

    //unsigned width = static_cast<unsigned>(map.width());
    //unsigned height = static_cast<unsigned>(map.height());
    //unsigned tile_size = static_cast<unsigned>(tile_size_i);
    std::vector<unsigned char> bytes(width * height * tile_size * tile_size * 4, 0xFF);
    for (auto && pair : map.coords_values()) {
        unsigned char val = (pair.value.type == mapgen::MapTileType::Ground) ? 0xFF : 0x00;
                                //? static_cast<unsigned char>(0xFF)
                                //: static_cast<unsigned char>(0x00);
        for (int32_t j = 0; j < tile_size; ++j) {
            auto y = (pair.coord.y * tile_size + j) * width * tile_size;
            for (int32_t i = 0; i < tile_size; ++i) {
                auto x = pair.coord.x * tile_size + i;
                auto idx = (y + x) * 4;
                bytes[idx + 0] = val;
                bytes[idx + 1] = val;
                bytes[idx + 2] = val;
            }
        }
    }
    return bytes;
}

}
