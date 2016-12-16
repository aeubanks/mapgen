//
// Created by Arthur Eubanks on 20/12/15.
//

#ifndef MAP_GEN_MAP_H
#define MAP_GEN_MAP_H


#include "mapgen_global.hpp"
#include "MapTileType.hpp"
#include "MapTile.hpp"
#include <ostream>
#include <functional>

namespace mapgen {
	class Map {
	public:
		// all of these are assuming that all neighbors are valid

		// the number of moore neighbors at most dist distance away
		static int num_neighbors_moore(int dist);
		// the total weight of moore neighbors at most dist distance away, closer neighbors have larger weight
		// e.g. for dist 3, anything moore neighbors of dist 1 have weight 3, any moore neighbors of dist 3 have weight 1
		static int num_neighbors_moore_weighted(int dist);
		// the number of von neumann neighbors at most dist distance away
		static int num_neighbors_von_neumann(int dist);
		// the total weight of von neumann neighbors at most dist distance away, closer neighbors have larger weight
		// e.g. for dist 3, anything von neumann neighbors of dist 1 have weight 3, any von neumann neighbors of dist 3 have weight 1
		static int num_neighbors_von_neumann_weighted(int dist);

		Map(int width, int height);
		Map(int width, int height, MapTileType type);
		Map(int width, int height, bool wrap);
		Map(int width, int height, bool wrap, MapTileType type);

		int width() const { return map_.width(); }
		int height() const { return map_.height(); }
		bool wrapped() const { return wrap_; }

	private:
		Array2D<MapTile> map_;
		bool wrap_;

	public:
		// iterators
		decltype(map_.begin()) begin() { return map_.begin(); }
		decltype(map_.end()) end() { return map_.end(); }
		decltype(map_.cbegin()) begin() const { return map_.cbegin(); }
		decltype(map_.cend()) end() const { return map_.cend(); }
		decltype(map_.cbegin()) cbegin() const { return map_.cbegin(); }
		decltype(map_.cend()) cend() const { return map_.cend(); }

		// returns an iterable that goes through all possible map coords
		decltype(map_.coords()) coords() const { return map_.coords(); }

		// returns an iterable that goes through all possible map coords
		decltype(map_.coords_values()) coords_values() { return map_.coords_values(); }
		auto coords_values() const -> decltype(map_.coords_values()) { return map_.coords_values(); }

		// returns if the coord is a valid coord to index the map
		bool in_bounds(Coord2D coord) const;

		// returns the MapTile at the coord
		MapTile & operator[](Coord2D coord);
		const MapTile & operator[](Coord2D coord) const;

		// returns the total number of tiles that are the given type
		int num_tiles_of_type(MapTileType tile) const;

		// returns the neighbors of a coord, filtering out out of bound ones
		vector<Coord2D> neighbors_moore(Coord2D coord) const;
		vector<Coord2D> neighbors_moore(Coord2D coord, int dist) const;
		vector<Coord2D> neighbors_von_neumann(Coord2D coord) const;
		vector<Coord2D> neighbors_von_neumann(Coord2D coord, int dist) const;

		using NeighborsFuncType = std::function<void(Coord2D)>;
		using NeighborsWeightedFuncType = std::function<void(Coord2D, int)>;

		// do something on each neighbor, not including out of bound ones
		// more efficient than getting the vector of neighbors (probably)
		void neighbors_moore_for_each(Coord2D coord, NeighborsFuncType neighbors_func) const;
		void neighbors_moore_for_each(Coord2D coord, int dist, NeighborsFuncType neighbors_func) const;
		void neighbors_moore_weighted_for_each(Coord2D coord, int dist, NeighborsWeightedFuncType neighbors_func) const;
		void neighbors_von_neumann_for_each(Coord2D coord, NeighborsFuncType neighbors_func) const;
		void neighbors_von_neumann_for_each(Coord2D coord, int dist, NeighborsFuncType neighbors_func) const;
		void neighbors_von_neumann_weighted_for_each(Coord2D coord, int dist, NeighborsWeightedFuncType neighbors_func) const;

		// returns the number of neighbors that have the specified MapTileType
		int num_neighbors_of_type_moore(MapTileType type, Coord2D coord) const;
		int num_neighbors_of_type_moore(MapTileType type, Coord2D coord, int dist) const;
		int num_neighbors_of_type_von_neumann(MapTileType type, Coord2D coord) const;
		int num_neighbors_of_type_von_neumann(MapTileType type, Coord2D coord, int dist) const;

		// returns all coords in the section that coord is part of
		// a section is a bunch of connected tiles that have the same type
		vector<Coord2D> find_section_moore(Coord2D coord) const;
		vector<Coord2D> find_section_von_neumann(Coord2D coord) const;


		// calls f(map, section) for each section, where section isa vector<Coord2D>
		void sections_for_each_von_neumann(MapTileType type, std::function<void(Map &, vector<Coord2D> &)> f);
		// calls f(map, section) for each section, where section isa vector<Coord2D>
		void sections_for_each_moore(MapTileType type, std::function<void(Map &, vector<Coord2D> &)> f);

		// flood fills the section with the specified MapTileType
		void flood_fill_von_neumann(Coord2D coord, MapTileType flood_type);
		void flood_fill_moore(Coord2D coord, MapTileType flood_type);

	private:
		// returns the wrapped coord (only if wrapped())
		Coord2D wrapped_coord(Coord2D coord) const;

		// helper function for the flood_fill methods
		void flood_fill_helper(Coord2D coord, MapTileType tile, vector<Coord2D>(Map::* neighbors_func)(Coord2D) const);

		// helper function for the find_section methods
		vector<Coord2D> find_section_helper(Coord2D coord, vector<Coord2D>(Map::* neighbors_func)(Coord2D) const) const;

		// calls f(map, section) for each section, where section isa vector<Coord2D>
		using SectionFuncType = vector<Coord2D>(Map::*)(Coord2D coord) const;
		void sections_for_each_helper(MapTileType type, std::function<void(Map &, vector<Coord2D> &)> f, SectionFuncType section_func);

		using NeighborsForEachFuncType = void(Map::*)(Coord2D coord, int dist, NeighborsFuncType neighbors_func) const;
		vector<Coord2D> neighbors_helper(Coord2D coord, int dist, NeighborsForEachFuncType find_neighbors_func) const;
	};

	std::ostream & operator<<(std::ostream & os, const Map & map);
}


#endif //MAP_GEN_MAP_H
