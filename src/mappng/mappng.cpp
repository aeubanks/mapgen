#include "mappng/mappng.hpp"

#include <png++/png.hpp>

namespace mappng {
void map_to_png(const std::string & file_name, const mapgen::Map & map, int tile_size) {
    int width = map.width();
    int height = map.height();
    png::image<png::rgb_pixel> image(width * tile_size, height * tile_size);

    for (auto && pair : map.coords_values()) {
        png::byte val = (pair.value.type == mapgen::MapTileType::Ground) ? 0xFF : 0x00;
        png::rgb_pixel pixel{val, val, val};
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
