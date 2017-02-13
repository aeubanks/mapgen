#pragma once

#include "MapGenerator.hpp"

namespace mapgen {
class DrunkMiners final : public MapGenerator {
  public:
    DrunkMiners(mg_util::Random & r, double spawn_prob, int32_t neighbor_ground_limit)
        : MapGenerator(r), spawn_prob_(spawn_prob),
          neighbor_ground_limit_(neighbor_ground_limit) {}

    void modify_map(Map & map) override;

  private:
    double spawn_prob_;
    int32_t neighbor_ground_limit_;
};
}

