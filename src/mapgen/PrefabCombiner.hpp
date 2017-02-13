#pragma once

#include "MapGenerator.hpp"

namespace mapgen {
class PrefabCombiner final : public MapGenerator {
public:
    PrefabCombiner(mg_util::Random & r, std::string roomsFileName);

    void modify_map(Map & map) override;

private:
    std::string roomsFileName_;
};
}

