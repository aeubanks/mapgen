#ifndef PREFAB_COMBINER_HPP
#define PREFAB_COMBINER_HPP

#include "MapGenerator.hpp"

namespace mapgen {
class PrefabCombiner : public MapGenerator {
public:
    PrefabCombiner(mg_util::Random & r, std::string roomsFileName);

    void modify_map(Map & map) override;

private:
    std::string roomsFileName_;
};
}

#endif
