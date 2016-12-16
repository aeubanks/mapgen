//
// Created by Arthur Eubanks on 29/12/15.
//

#ifndef MAPGEN_ROOMMAKER_HPP
#define MAPGEN_ROOMMAKER_HPP


#include "mapgen_global.hpp"

#include "Map.hpp"
#include "MapGenerator.hpp"
#include "mg_util/mg_math.hpp"

namespace mapgen {
	template<class MakeRoomDecider>
	class RoomMaker : public MapGenerator {
	public:
		RoomMaker(mg_util::Random & r, int tries, int min_width, int max_width, int min_height, int max_height, MakeRoomDecider make_room_decider = MakeRoomDecider()) : MapGenerator(r), tries_(tries), min_width_(min_width), max_width_(max_width), min_height_(min_height), max_height_(max_height), make_room_decider_(make_room_decider) { }

		void modify_map(Map & map) override;

	private:
		// the number of tries
		int tries_;
		// the min/max width/height of each room
		int min_width_, max_width_, min_height_, max_height_;
		// RoomMaker will decide whether to make a room or not based on make_room_decider_(map, x_start, y_start, width, height)
		MakeRoomDecider make_room_decider_;
	};

	template<class MakeRoomDecider>
	void RoomMaker<MakeRoomDecider>::modify_map(Map & map) {
		assert(min_width_ > 0);
		assert(min_height_ > 0);
		assert(min_width_ <= max_width_);
		assert(min_height_ <= max_height_);
		assert(max_width_ < map.width());
		assert(max_height_ < map.height());
		assert(tries_ > 0);

		for (int cur_try = 0; cur_try < tries_; ++cur_try) {
			int width = rand_.rand_int_exc(min_width_, max_width_ + 1);
			int height = rand_.rand_int_exc(min_height_, max_height_ + 1);
			int x_start = rand_.rand_int_exc(0, map.width() - width);
			int y_start = rand_.rand_int_exc(0, map.height() - height);
			if (make_room_decider_(map, x_start, y_start, width, height)) {
				for (int j = 0; j < height; ++j) {
					for (int i = 0; i < width; ++i) {
						int x = i + x_start;
						int y = j + y_start;
						map[{x, y}].type = MapTileType::Ground;
					}
				}
			}
		}
	}


	// always make a room
	class MakeRoomDeciderAlways {
	public:
		MakeRoomDeciderAlways() { }

		bool operator()(Map & /*map*/, int /*x*/, int /*y*/, int /*width*/, int /*height*/) {
			return true;
		}
	};


	// only make a room if the room plus an x and y border does not overlap with any other empty spaces
	class MakeRoomDeciderNoOverlap {
	public:
		MakeRoomDeciderNoOverlap() : MakeRoomDeciderNoOverlap(0) { }

		MakeRoomDeciderNoOverlap(int border) : MakeRoomDeciderNoOverlap(border, border) { }

		MakeRoomDeciderNoOverlap(int xborder, int yborder) : xborder_(xborder), yborder_(yborder) { }

		bool operator()(Map & map, int x_start, int y_start, int width, int height) {
			assert(xborder_ >= 0);
			assert(yborder_ >= 0);
			int x_start_with_border = x_start - xborder_;
			int y_start_with_border = y_start - yborder_;
			int x_end_with_border = x_start + width + xborder_;
			int y_end_with_border = y_start + height + yborder_;
			
			mg_util::clamp_inc(x_start_with_border, 0, map.width());
			mg_util::clamp_inc(y_start_with_border, 0, map.width());
			mg_util::clamp_inc(x_end_with_border, 0, map.width());
			mg_util::clamp_inc(y_end_with_border, 0, map.width());

			for (int y = y_start_with_border; y < y_end_with_border; ++y) {
				for (int x = x_start_with_border; x < x_end_with_border; ++x) {
					if (map[{x, y}].type == MapTileType::Ground) {
						return false;
					}
				}
			}

			return true;
		}

	private:
		int xborder_, yborder_;
	};
}


#endif //MAPGEN_ROOMMAKER_HPP
