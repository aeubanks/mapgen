//
// Created by Arthur Eubanks on 20/12/15.
//

#include "Map.hpp"

#include <algorithm>
#include <stack>
#include <utility>

namespace mapgen {
int Map::num_neighbors_moore(int dist) {
    int side = 2 * dist + 1;
    return side * side - 1;
}

int Map::num_neighbors_moore_weighted(int dist) {
    int count = 0;
    for (int i = 1; i <= dist; ++i) {
        count += 8 * i * (dist + 1 - i);
    }
    return count;
}

int Map::num_neighbors_von_neumann(int dist) { return 2 * dist * (dist + 1); }

int Map::num_neighbors_von_neumann_weighted(int dist) {
    int count = 0;
    for (int i = 1; i <= dist; ++i) {
        count += 4 * i * (dist + 1 - i);
    }
    return count;
}

Map::Map(int width, int height) : Map(width, height, false) {}

Map::Map(int width, int height, MapTileType type)
    : Map(width, height, false, type) {}

Map::Map(int width, int height, bool wrap)
    : Map(width, height, wrap, MapTileType::Wall) {}

Map::Map(int width, int height, bool wrap, MapTileType type)
    : map_(width, height, type), wrap_(wrap) {
    if constexpr (DEBUG) {
        if (width <= 0 || height <= 0) {
            mg_log::error("invalid width/height: ", width, ", ", height);
            throw mg_error("");
        }
    }
}

bool Map::in_bounds(Coord2D coord) const {
    return wrap_ || coord.in_bounds(width(), height());
}

Coord2D Map::wrapped_coord(Coord2D coord) const {
    if constexpr (DEBUG) {
        if (!in_bounds(coord) || !wrap_) {
            mg_log::error("invalid state in Map::wrapped_coord");
            throw mg_error("");
        }
    }

    coord.x %= width();
    if (coord.x < 0) {
        coord.x += width();
    }
    coord.y %= height();
    if (coord.y < 0) {
        coord.y += height();
    }
    return coord;
}

MapTile & Map::operator[](Coord2D coord) {
    if constexpr (DEBUG) {
        if (!in_bounds(coord)) {
            mg_log::error("invalid coord in Map::operator[]");
            throw mg_error("");
        }
    }

    if (wrap_) {
        coord = wrapped_coord(coord);
    }
    return map_[coord];
}

const MapTile & Map::operator[](Coord2D coord) const {
    return const_cast<Map *>(this)->operator[](coord);
}

int Map::num_tiles_of_type(MapTileType type) const {
    int count = 0;
    for (auto & t : map_) {
        if (t.type == type) {
            count++;
        }
    }
    return count;
}

int Map::num_neighbors_of_type_moore(MapTileType type, Coord2D coord, int dist) const {
    if constexpr (DEBUG) {
        if (!in_bounds(coord)) {
            mg_log::error("invalid coord in Map::num_neighbors_of_type_moore[]");
            throw mg_error("");
        }
    }

    int count = 0;
    neighbors_moore_for_each(coord, dist, [&count, type, this](Coord2D neighbor) {
        if (operator[](neighbor).type == type) {
            ++count;
        }
    });
    return count;
}

int Map::num_neighbors_of_type_moore(MapTileType type, Coord2D coord) const {
    return num_neighbors_of_type_moore(type, coord, 1);
}

int Map::num_neighbors_of_type_von_neumann(MapTileType type, Coord2D coord, int dist) const {
    if constexpr (DEBUG) {
        if (!in_bounds(coord)) {
            mg_log::error("invalid coord in Map::num_neighbors_of_type_von_neumann[]");
            throw mg_error("");
        }
    }

    int count = 0;
    neighbors_von_neumann_for_each(coord, dist,
                                   [&count, type, this](Coord2D neighbor) {
                                       if (operator[](neighbor).type == type) {
                                           ++count;
                                       }
                                   });
    return count;
}

int Map::num_neighbors_of_type_von_neumann(MapTileType type,
                                           Coord2D coord) const {
    return num_neighbors_of_type_von_neumann(type, coord, 1);
}

vector<Coord2D> Map::find_section_von_neumann(Coord2D coord) const {
    return find_section_helper(coord, &Map::neighbors_von_neumann);
}

vector<Coord2D> Map::find_section_moore(Coord2D coord) const {
    return find_section_helper(coord, &Map::neighbors_moore);
}

vector<Coord2D>
Map::find_section_helper(Coord2D coord, vector<Coord2D> (Map::*neighbors_func)(Coord2D) const) const {
    if constexpr (DEBUG) {
        if (!in_bounds(coord)) {
            mg_log::error("invalid coord in Map::find_section_helper[]");
            throw mg_error("");
        }
    }

    vector<Coord2D> ret;
    Array2D<bool> visited(width(), height());
    auto type = operator[](coord).type;

    std::stack<Coord2D, vector<Coord2D>> stack;
    stack.push(coord);
    visited[coord] = true;
    while (!stack.empty()) {
        auto cur = stack.top();
        stack.pop();

        ret.push_back(cur);
        for (auto & neighbor : (this->*neighbors_func)(cur)) {
            auto & cur_tile = operator[](neighbor);
            if (!visited[neighbor] && cur_tile.type == type) {
                visited[neighbor] = true;
                stack.push(neighbor);
            }
        }
    }

    return ret;
}

void Map::flood_fill_von_neumann(Coord2D coord, MapTileType flood_type) {
    flood_fill_helper(coord, flood_type, &Map::neighbors_von_neumann);
}

void Map::flood_fill_moore(Coord2D coord, MapTileType flood_type) {
    flood_fill_helper(coord, flood_type, &Map::neighbors_moore);
}

void Map::flood_fill_helper(Coord2D coord, MapTileType flood_type, vector<Coord2D> (Map::*neighbors_func)(Coord2D) const) {
    if constexpr (DEBUG) {
        if (!in_bounds(coord)) {
            mg_log::error("invalid coord in Map::flood_fill_helper[]");
            throw mg_error("");
        }
    }

    MapTileType overwrite_type = operator[](coord).type;
    if (overwrite_type ==
        flood_type) { // if flooding with the same type that's already there, exit
        return;
    }

    Array2D<bool> visited(width(), height());

    std::stack<Coord2D, vector<Coord2D>> stack;
    stack.push(coord);
    while (!stack.empty()) {
        auto cur = stack.top();
        stack.pop();
        for (auto & neighbor : (this->*neighbors_func)(cur)) {
            auto & cur_tile = (*this)[neighbor];
            if (!visited[neighbor] && cur_tile.type == overwrite_type) {
                visited[neighbor] = true;
                cur_tile.type = flood_type;
                stack.push(neighbor);
            }
        }
    }
}

vector<Coord2D> Map::neighbors_moore(Coord2D coord) const {
    return neighbors_moore(coord, 1);
}

vector<Coord2D> Map::neighbors_moore(Coord2D coord, int dist) const {
    return neighbors_helper(coord, dist, &Map::neighbors_moore_for_each);
    //		vector<Coord2D> ret;
    //		auto add_to_ret = [&ret, this](Coord2D add_coord) {
    //			ret.push_back(add_coord);
    //		};
    //		neighbors_moore_for_each(coord, dist, add_to_ret);
    //		return ret;
}

vector<Coord2D> Map::neighbors_von_neumann(Coord2D coord) const {
    return neighbors_von_neumann(coord, 1);
}

vector<Coord2D> Map::neighbors_von_neumann(Coord2D coord, int dist) const {
    return neighbors_helper(coord, dist, &Map::neighbors_von_neumann_for_each);
    //		vector<Coord2D> ret;
    //		auto add_to_ret = [&ret, this](Coord2D add_coord) {
    //			ret.push_back(add_coord);
    //		};
    //		neighbors_von_neumann_for_each(coord, dist, add_to_ret);
    //		return ret;
}

vector<Coord2D>
Map::neighbors_helper(Coord2D coord, int dist,
                      NeighborsForEachFuncType find_neighbors_func) const {
    vector<Coord2D> ret;
    auto add_to_ret = [&ret, this](Coord2D add_coord) {
        ret.push_back(add_coord);
    };
    (this->*find_neighbors_func)(coord, dist, add_to_ret);
    return ret;
}

void Map::neighbors_moore_for_each(Coord2D coord,
                                   NeighborsFuncType neighbors_func) const {
    neighbors_moore_for_each(coord, 1, neighbors_func);
}

void Map::neighbors_moore_for_each(Coord2D coord, int dist, NeighborsFuncType neighbors_func) const {
    if constexpr (DEBUG) {
        if (dist <= 0) {
            mg_log::error("invalid dist in Map::neighbors_moore_for_each[]");
            throw mg_error("");
        }
        if (!in_bounds(coord)) {
            mg_log::error("invalid coord in Map::neighbors_moore_for_each[]");
            throw mg_error("");
        }
    }

    for (int i = -dist; i <= dist; i++) {
        for (int j = -dist; j <= dist; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            auto check_coord = coord.offseted(i, j);
            if (in_bounds(check_coord)) {
                neighbors_func(check_coord);
            }
        }
    }
}

void Map::neighbors_moore_weighted_for_each(
    Coord2D coord, int dist, NeighborsWeightedFuncType neighbors_func) const {
    auto find_weight_and_neighbors_func = [coord, dist, neighbors_func,
                                           this](Coord2D idx) {
        auto dif = coord - idx;
        int weight = dist + 1 - std::max(abs(dif.x), abs(dif.y));
        neighbors_func(idx, weight);
    };
    neighbors_moore_for_each(coord, dist, find_weight_and_neighbors_func);
}

void Map::neighbors_von_neumann_for_each(
    Coord2D coord, NeighborsFuncType neighbors_func) const {
    neighbors_von_neumann_for_each(coord, 1, neighbors_func);
}

void Map::neighbors_von_neumann_for_each(Coord2D coord, int dist, NeighborsFuncType neighbors_func) const {
    if constexpr (DEBUG) {
        if (dist <= 0) {
            mg_log::error("invalid dist in Map::neighbors_von_neumann_for_each[]");
            throw mg_error("");
        }
        if (!in_bounds(coord)) {
            mg_log::error("invalid coord in Map::neighbors_von_neumann_for_each[]");
            throw mg_error("");
        }
    }

    int up_down = 0;
    for (int i = -dist; i <= dist; ++i) {
        for (int j = -up_down; j <= up_down; ++j) {
            if (i == 0 && j == 0) {
                continue;
            }
            auto check_coord = coord.offseted(i, j);
            if (in_bounds(check_coord)) {
                neighbors_func(check_coord);
            }
        }
        if (i < 0) {
            ++up_down;
        } else {
            --up_down;
        }
    }
}

void Map::neighbors_von_neumann_weighted_for_each(
    Coord2D coord, int dist, NeighborsWeightedFuncType neighbors_func) const {
    auto find_weight_and_neighbors_func = [coord, dist, neighbors_func,
                                           this](Coord2D idx) {
        auto dif = coord - idx;
        int weight = 2 * dist + 1 - (abs(dif.x) + abs(dif.y));
        neighbors_func(idx, weight);
    };
    neighbors_moore_for_each(coord, dist, find_weight_and_neighbors_func);
}

void Map::sections_for_each_von_neumann(
    MapTileType type, std::function<void(Map &, vector<Coord2D> &)> f) {
    sections_for_each_helper(type, f, &Map::find_section_von_neumann);
}

void Map::sections_for_each_moore(
    MapTileType type, std::function<void(Map &, vector<Coord2D> &)> f) {
    sections_for_each_helper(type, f, &Map::find_section_moore);
}

// calls f(map, section) for each section, where section isa vector<Coord2D>
void Map::sections_for_each_helper(
    MapTileType type, std::function<void(Map &, vector<Coord2D> &)> f,
    SectionFuncType section_func) {
    Array2D<bool> visited(width(), height());
    for (auto && item : this->coords_values()) {
        if (!visited[item.coord] && item.value.type == type) {
            auto section = (this->*section_func)(item.coord);
            std::for_each(
                section.begin(), section.end(),
                [&visited](const Coord2D & coord) { visited[coord] = true; });
            f(*this, section);
        }
    }
}

std::ostream & operator<<(std::ostream & os, const Map & map) {
    for (auto && item : map.coords_values()) {
        os << MapTileType_to_char(item.value.type);
        if (item.coord.x == map.width() - 1 && item.coord.y != map.height() - 1) {
            os << '\n';
        }
    }

    return os;
}
}
