#include "mappng/mappng.hpp"

#include <png++/png.hpp>

namespace mappng {
static png::rgb_pixel getPixelForType(mapgen::MapTileType type) {
    using namespace mapgen;
    switch (type) {
        case MapTileType::None:
            return {0xFF, 0x00, 0xFF};
        case MapTileType::Connection:
            return {0x66, 0x00, 0xCC};
        case MapTileType::Wall:
            return {0x00, 0x00, 0x00};
        case MapTileType::Ground:
            return {0x80, 0x80, 0x80};
        case MapTileType::Grass:
            return {0x33, 0xFF, 0x33};
        case MapTileType::Bush:
            return {0x00, 0xCC, 0x00};
        case MapTileType::Forest:
            return {0x00, 0x66, 0x00};
        case MapTileType::Mountain:
            return {0xC0, 0xC0, 0xC0};
        case MapTileType::Door:
            return {0x66, 0x33, 0x00};
        default:
            mg_log::error("unexpected tile for getPixelForType: ", MapTileType_to_string(type));
            return {0x00, 0x00, 0x00};

    }
}

void map_to_png(const std::string & file_name, const mapgen::Map & map, int tile_size) {
    int width = map.width();
    int height = map.height();
    png::image<png::rgb_pixel> image(width * tile_size, height * tile_size);

    for (auto && pair : map.coords_values()) {
        //png::byte val = (pair.value.type == mapgen::MapTileType::Ground) ? 0xFF : 0x00;
        //png::rgb_pixel pixel{val, val, val};
        png::rgb_pixel pixel = getPixelForType(pair.value.type);

        for (int j = 0; j < tile_size; ++j) {
            auto y = pair.coord.y * tile_size + j;
            for (int i = 0; i < tile_size; ++i) {
                auto x = pair.coord.x * tile_size + i;
                image.set_pixel(x, y, pixel);
            }
        }
    }
    image.write(file_name);
}
}
