//
// Created by Arthur Eubanks on 20/12/15.
//

#ifndef MAP_GEN_MAP_H
#define MAP_GEN_MAP_H

#include "MapTile.hpp"
#include "MapTileType.hpp"
#include "mapgen_global.hpp"
#include <functional>
#include <ostream>

namespace mapgen {
class Map {
  public:
    using SizeType = int32_t;
    using DistType = int32_t;
    using CountType = int32_t;

    // all of these are assuming that all neighbors are valid

    // the number of moore neighbors at most dist distance away
    static CountType num_neighbors_moore(DistType dist);
    // the total weight of moore neighbors at most dist distance away, closer
    // neighbors have larger weight
    // e.g. for dist 3, anything moore neighbors of dist 1 have weight 3, any
    // moore neighbors of dist 3 have weight 1
    static CountType num_neighbors_moore_weighted(DistType dist);
    // the number of von neumann neighbors at most dist distance away
    static CountType num_neighbors_von_neumann(DistType dist);
    // the total weight of von neumann neighbors at most dist distance away,
    // closer neighbors have larger weight
    // e.g. for dist 3, anything von neumann neighbors of dist 1 have weight 3,
    // any von neumann neighbors of dist 3 have weight 1
    static CountType num_neighbors_von_neumann_weighted(DistType dist);

    Map(SizeType width, SizeType height);
    Map(SizeType width, SizeType height, MapTileType type);
    Map(SizeType width, SizeType height, bool wrap);
    Map(SizeType width, SizeType height, bool wrap, MapTileType type);

    SizeType width() const { return map_.width(); }
    SizeType height() const { return map_.height(); }
    bool wrapped() const { return wrap_; }

  private:
    Array2D<MapTile> map_;
    bool wrap_;

  public:
    // iterators
    auto begin() { return map_.begin(); }
    auto end() { return map_.end(); }
    auto begin() const { return map_.cbegin(); }
    auto end() const { return map_.cend(); }
    auto cbegin() const { return map_.cbegin(); }
    auto cend() const { return map_.cend(); }

    // returns an iterable that goes through all possible map coords
    auto coords() const { return map_.coords(); }

    // returns an iterable that goes through all possible map coords and values
    auto coords_values() { return map_.coords_values(); }
    auto coords_values() const { return map_.coords_values(); }

    // returns an iterable that goes through all possible map values
    auto values() { return map_.values(); }
    auto values() const { return map_.values(); }

    // returns if the coord is a valid coord to index the map
    bool in_bounds(Coord2D coord) const;

    // returns the MapTile at the coord
    MapTile & operator[](Coord2D coord);
    const MapTile & operator[](Coord2D coord) const;

    Map flippedX() const;
    Map flippedY() const;
    Map rotated() const;

    // returns the total number of tiles that are the given type
    CountType num_tiles_of_type(MapTileType tile) const;

    // returns the neighbors of a coord, filtering out out of bound ones
    vector<Coord2D> neighbors_moore(Coord2D coord) const;
    vector<Coord2D> neighbors_moore(Coord2D coord, DistType dist) const;
    vector<Coord2D> neighbors_von_neumann(Coord2D coord) const;
    vector<Coord2D> neighbors_von_neumann(Coord2D coord, DistType dist) const;

    using NeighborsFuncType = std::function<void(Coord2D)>;
    using NeighborsWeightedFuncType = std::function<void(Coord2D, DistType)>;

    // do something on each neighbor, not including out of bound ones
    // more efficient than getting the vector of neighbors (probably)
    void neighbors_moore_for_each(Coord2D coord, NeighborsFuncType neighbors_func) const;
    void neighbors_moore_for_each(Coord2D coord, DistType dist, NeighborsFuncType neighbors_func) const;
    void neighbors_moore_weighted_for_each(Coord2D coord, DistType dist, NeighborsWeightedFuncType neighbors_func) const;
    void neighbors_von_neumann_for_each(Coord2D coord, NeighborsFuncType neighbors_func) const;
    void neighbors_von_neumann_for_each(Coord2D coord, DistType dist, NeighborsFuncType neighbors_func) const;
    void neighbors_von_neumann_weighted_for_each(Coord2D coord, DistType dist, NeighborsWeightedFuncType neighbors_func) const;

    // returns the number of neighbors that have the specified MapTileType
    CountType num_neighbors_of_type_moore(MapTileType type, Coord2D coord) const;
    CountType num_neighbors_of_type_moore(MapTileType type, Coord2D coord, DistType dist) const;
    CountType num_neighbors_of_type_von_neumann(MapTileType type, Coord2D coord) const;
    CountType num_neighbors_of_type_von_neumann(MapTileType type, Coord2D coord, DistType dist) const;

    // returns all coords in the section that coord is part of
    // a section is a bunch of connected tiles that have the same type
    vector<Coord2D> find_section_moore(Coord2D coord) const;
    vector<Coord2D> find_section_von_neumann(Coord2D coord) const;

    // calls f(map, section) for each section, where section isa vector<Coord2D>
    void sections_for_each_von_neumann(
        MapTileType type, std::function<void(Map &, vector<Coord2D> &)> f);
    // calls f(map, section) for each section, where section isa vector<Coord2D>
    void sections_for_each_moore(MapTileType type,
                                 std::function<void(Map &, vector<Coord2D> &)> f);

    // flood fills the section with the specified MapTileType
    void flood_fill_von_neumann(Coord2D coord, MapTileType flood_type);
    void flood_fill_moore(Coord2D coord, MapTileType flood_type);

  private:
    // returns the wrapped coord (only if wrapped())
    Coord2D wrapped_coord(Coord2D coord) const;

    // helper function for the flood_fill methods
    void flood_fill_helper(Coord2D coord, MapTileType tile,
                           vector<Coord2D> (Map::*neighbors_func)(Coord2D) const);

    // helper function for the find_section methods
    vector<Coord2D>
    find_section_helper(Coord2D coord,
                        vector<Coord2D> (Map::*neighbors_func)(Coord2D)
                            const) const;

    // calls f(map, section) for each section, where section isa vector<Coord2D>
    using SectionFuncType = vector<Coord2D> (Map::*)(Coord2D coord) const;
    void sections_for_each_helper(MapTileType type,
                                  std::function<void(Map &, vector<Coord2D> &)> f,
                                  SectionFuncType section_func);

    using NeighborsForEachFuncType = void (Map::*)(
        Coord2D coord, DistType dist, NeighborsFuncType neighbors_func) const;
    vector<Coord2D>
    neighbors_helper(Coord2D coord, DistType dist,
                     NeighborsForEachFuncType find_neighbors_func) const;
};

std::ostream & operator<<(std::ostream & os, const Map & map);
}

#endif // MAP_GEN_MAP_H
