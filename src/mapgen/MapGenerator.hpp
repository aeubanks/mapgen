//
// Created by Arthur Eubanks on 20/12/15.
//

#ifndef MAP_GEN_MAPGENERATOR_H
#define MAP_GEN_MAPGENERATOR_H

#include "mapgen_global.hpp"

#include <type_traits>
#include <utility>

#include "Map.hpp"
#include "MapTileType.hpp"
#include "mg_util/Random.hpp"

namespace mapgen {
// the base class for anything that wants to modify a map
class MapGenerator {
  public:
    MapGenerator(mg_util::Random & r) : rand_(r) {}

    // this is what the user should call to modify a map
    void operator()(Map & map) { modify_map(map); }

  private:
    // this is the virtual, abstract method that should be overridden
    virtual void modify_map(Map & map) = 0;

  protected:
    // a reference to a Random so that if multiple MapGenerators are used, they
    // can use the same one and be deterministic
    mg_util::Random & rand_;

    // destructor is protected so that subclasses cannot be destroyed through a
    // pointer to the base class
    virtual ~MapGenerator() {}
};

/*
namespace {

template <typename... Ts>
void _gen(Map & map, Ts &&... args);

template <typename T, typename... Ts>
void _gen_helper(Map & map, T && t, Ts &&... args) {
    static_assert(std::is_base_of<MapGenerator,
                                  typename std::remove_reference<T>::type>::value,
                  "must use subclasses of MapGenerator in create_map()");
    t(map);
    _gen<Ts...>(map, std::forward<Ts>(args)...);
}

template <typename... Ts>
void _gen(Map & map, Ts &&... args) {
    _gen_helper<Ts...>(map, std::forward<Ts>(args)...);
}

template <>
void _gen(Map &);

}
*/

// use this to create a map that is modified by each of the MapGenerators
template <typename... Ts>
Map create_map(int width, int height, bool wrap, Ts &&... args) {
    Map map(width, height, wrap);
    (args(map), ...);
    //_gen<Ts...>(map, std::forward<Ts>(args)...);
    return map;
}
}

#endif // MAP_GEN_MAPGENERATOR_H
