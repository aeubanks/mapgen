#pragma once

#include "Map.hpp"
#include "MapGenerator.hpp"
#include "mapgen_include.hpp"

namespace mapgen {
// randomizes every tile
class Randomizer final : public MapGenerator {
  public:
    // constructor, chance should be probability that a tile will be Ground
    Randomizer(mg_util::Random & r, double chance)
        : MapGenerator(r), chance_(chance) {}
    void modify_map(Map & map) override;

  private:
    // the chance a tile will be Ground
    double chance_;
};
}

