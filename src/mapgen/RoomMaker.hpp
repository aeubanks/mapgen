#pragma once

#include "mapgen_include.hpp"

#include "Map.hpp"
#include "MapGenerator.hpp"
#include "mg_util/mg_math.hpp"

namespace mapgen {
template <class MakeRoomDecider>
class RoomMaker final : public MapGenerator {
  public:
    RoomMaker(mg_util::Random & r, Map::SizeType tries, Map::SizeType min_width, Map::SizeType max_width,
              Map::SizeType min_height, Map::SizeType max_height,
              MakeRoomDecider make_room_decider = MakeRoomDecider())
        : MapGenerator(r), tries_(tries), min_width_(min_width),
          max_width_(max_width), min_height_(min_height), max_height_(max_height),
          make_room_decider_(make_room_decider) {}

    void modify_map(Map & map) override;

  private:
    // the number of tries
    int32_t tries_;
    // the min/max width/height of each room
    Map::SizeType min_width_, max_width_, min_height_, max_height_;
    // RoomMaker will decide whether to make a room or not based on
    // make_room_decider_(map, x_start, y_start, width, height)
    MakeRoomDecider make_room_decider_;
};

template <class MakeRoomDecider>
void RoomMaker<MakeRoomDecider>::modify_map(Map & map) {
    if constexpr (DEBUG) {
        if (min_width_ <= 0 || min_height_ <= 0 || min_width_ > max_width_ || min_height_ > max_height_ || max_width_ > map.width() || max_height_ > map.height() || tries_ <= 0) {
            mg_log::error("invalid state for RoomMaker::modify_map");
            throw mg_error("");
        }
    }

    for (int32_t cur_try = 0; cur_try < tries_; ++cur_try) {
        Map::SizeType width = rand_.rand_int_exc(min_width_, max_width_ + 1);
        Map::SizeType height = rand_.rand_int_exc(min_height_, max_height_ + 1);
        Map::SizeType x_start = rand_.rand_int_exc(0, map.width() - width);
        Map::SizeType y_start = rand_.rand_int_exc(0, map.height() - height);
        if (make_room_decider_(map, x_start, y_start, width, height)) {
            for (Map::SizeType j = 0; j < height; ++j) {
                for (Map::SizeType i = 0; i < width; ++i) {
                    Map::SizeType x = i + x_start;
                    Map::SizeType y = j + y_start;
                    map[{x, y}].type = MapTileType::Ground;
                }
            }
        }
    }
}

// always make a room
class MakeRoomDeciderAlways {
  public:
    MakeRoomDeciderAlways() {}

    bool operator()(Map & /*map*/, Map::SizeType /*x*/, Map::SizeType /*y*/, Map::SizeType /*width*/,
                    Map::SizeType /*height*/) {
        return true;
    }
};

// only make a room if the room plus an x and y border does not overlap with any
// other empty spaces
class MakeRoomDeciderNoOverlap {
  public:
    MakeRoomDeciderNoOverlap() : MakeRoomDeciderNoOverlap(0) {}

    MakeRoomDeciderNoOverlap(Map::SizeType border)
        : MakeRoomDeciderNoOverlap(border, border) {}

    MakeRoomDeciderNoOverlap(Map::SizeType xborder, Map::SizeType yborder)
        : xborder_(xborder), yborder_(yborder) {}

    bool operator()(Map & map, Map::SizeType x_start, Map::SizeType y_start, Map::SizeType width, Map::SizeType height) {
        if constexpr (DEBUG) {
            if (xborder_ < 0 || yborder_ < 0) {
                mg_log::error("invalid xborder/yborder: ", xborder_, ", ", yborder_);
                throw mg_error("");
            }
        }

        Map::SizeType x_start_with_border = x_start - xborder_;
        Map::SizeType y_start_with_border = y_start - yborder_;
        Map::SizeType x_end_with_border = x_start + width + xborder_;
        Map::SizeType y_end_with_border = y_start + height + yborder_;

        x_start_with_border = mg_util::clamp_inc(x_start_with_border, 0, map.width());
        y_start_with_border = mg_util::clamp_inc(y_start_with_border, 0, map.width());
        x_end_with_border = mg_util::clamp_inc(x_end_with_border, 0, map.width());
        y_end_with_border = mg_util::clamp_inc(y_end_with_border, 0, map.width());

        for (Map::SizeType y = y_start_with_border; y < y_end_with_border; ++y) {
            for (Map::SizeType x = x_start_with_border; x < x_end_with_border; ++x) {
                if (map[{x, y}].type == MapTileType::Ground) {
                    return false;
                }
            }
        }

        return true;
    }

  private:
    Map::SizeType xborder_, yborder_;
};
}

