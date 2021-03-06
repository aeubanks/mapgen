#pragma once

#include "MapGenerator.hpp"

namespace mapgen {
template <typename CARule>
class CellularAutomaton final : public MapGenerator {
  public:
    // sets rule to use the default constructor if none is specified
    CellularAutomaton(mg_util::Random & r, int32_t generations, CARule rule = CARule())
        : MapGenerator(r), generations_(generations), rule_(rule) {}

    void modify_map(Map & map) override;

  private:
    // runs one generation on the map
    // goes through every coord, calls rule_(map, coord), if true, sets that to
    // Ground; if false, sets that to Wall
    void generation(Map & map);

    int32_t generations_; // the number of generations to run the cellular automaton
    CARule rule_;     // the rule for
};

template <typename CARule>
void CellularAutomaton<CARule>::modify_map(Map & map) {
    for (int32_t i = 0; i < generations_; i++) {
        generation(map);
    }
}

template <typename CARule>
void CellularAutomaton<CARule>::generation(Map & map) {
    Map next_gen = map;
    for (auto coord : map.coords()) {
        next_gen[coord] = rule_(map, coord) ? MapTileType::Ground : MapTileType::Wall;
    }
    map = next_gen;
}

// a possible CARule for CellularAutomaton
// if a tile is alive, returns true if the number of alive neighbors >=
// alive_limit_
// if a tile is dead, returns true if the number of alive neighbors >=
// dead_limit_
class CARuleAliveDeadLimit {
  public:
    CARuleAliveDeadLimit(int32_t alive_limit, int32_t dead_limit)
        : alive_limit_(alive_limit), dead_limit_(dead_limit) {}

    bool operator()(Map & map, Coord2D coord);

  private:
    int32_t alive_limit_, dead_limit_;
};
}

