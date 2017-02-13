#pragma once

#include "MapGenerator.hpp"
#include "mapgen_include.hpp"

namespace mapgen {
// MapGenerator that fills in sections, determined by fill_(map, section) where
// section isa vector<Coord2D>
template <class Fill>
class SectionRemover final : public MapGenerator {
  public:
    SectionRemover(mg_util::Random & r, MapTileType type_to_fill,
                   MapTileType type_to_fill_with, Fill fill = Fill())
        : MapGenerator(r), type_to_fill_(type_to_fill),
          type_to_fill_with_(type_to_fill_with), fill_(fill) {}

    void modify_map(Map & map) override;

  private:
    // the type to fill and the type to fill it with
    MapTileType type_to_fill_, type_to_fill_with_;
    // the rule on whether to fill in a section or not
    Fill fill_;
};

template <class Fill>
void SectionRemover<Fill>::modify_map(Map & map) {
    int32_t num_sections = 0;
    int32_t num_sections_removed = 0;

    auto & fill = fill_;
    auto type_to_fill_with = type_to_fill_with_;

    auto remove_if_small = [&num_sections_removed, &num_sections, &fill,
                            type_to_fill_with](Map & map_lambda,
                                               std::vector<Coord2D> section) {
        if (fill(map_lambda, section)) {
            auto set_to_wall = [&map_lambda, type_to_fill_with](Coord2D coord) {
                map_lambda[coord].type = type_to_fill_with;
            };
            std::for_each(section.begin(), section.end(), set_to_wall);
            num_sections_removed++;
        }
        num_sections++;
    };
    map.sections_for_each_moore(type_to_fill_, remove_if_small);
    mg_log::info("num sections:         {}", num_sections);
    mg_log::info("num sections removed: {}", num_sections_removed);
    mg_log::info("num sections kept:    {}", num_sections - num_sections_removed);
}

// returns (section.size() * factor < num tiles)
// basically the section size * factor has to be at least the total number of
// tiles to remain
// larger factor means smaller sections remain
// smaller factor means larger sections remain
class SectionRemoverSmall {
  public:
    SectionRemoverSmall(int32_t factor) : factor_(factor) {}

    bool operator()(const Map & map, std::vector<Coord2D> & section) {
        return static_cast<int32_t>(section.size()) * factor_ < map.width() * map.height();
    }

  private:
    int32_t factor_;
};
}

