#pragma once

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

