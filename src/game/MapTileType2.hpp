//
// Created by I7 on 5/16/2016.
//

#ifndef GAME_MAPTILETYPE2_HPP
#define GAME_MAPTILETYPE2_HPP

#include <string>
#include <vector>

namespace game {
class MapTileType2 {
  private:
    static int cur_id;
    static std::vector<MapTileType2> types;

  public:
    static void init();
    static int num_types();
    static const MapTileType2 & get_type(int id);

  public:
    MapTileType2(int id, std::string & name, int x_tile, int y_tile);
    MapTileType2 & operator=(const MapTileType2 &) = delete;
    int id() const;
    const std::string & name() const;
    int x_tile() const;
    int y_tile() const;

  private:
    int id_;
    std::string name_;
    int x_tile_, y_tile_;
};
}

#endif // GAME_MAPTILETYPE2_HPP
