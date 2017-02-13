#include "PrefabCombiner.hpp"

#include <sstream>
#include <utility>
#include <fstream>
#include <iterator>
#include <set>
#include <map>
#include <tuple>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace std {
template<>
struct std::less<mg_util::Coord2D> {
    bool operator()(const mg_util::Coord2D & a, const mg_util::Coord2D & b) const {
        return std::less<decltype(std::tie(a.x, a.y))>()(std::tie(a.x, a.y), std::tie(b.x, b.y));
    }
};

template<typename T>
struct std::less<std::pair<const T *, mg_util::Coord2D>> {
    bool operator()(const std::pair<const T *, mg_util::Coord2D> & a, const std::pair<const T *, mg_util::Coord2D> & b) const {
        if (a.first != b.first) {
            return std::less<const T *>()(a.first, b.first);
        }
        return std::less<mg_util::Coord2D>()(a.second, b.second);
    }
};

template<>
struct std::less<mapgen::Map> {
    bool operator()(const mapgen::Map & a, const mapgen::Map & b) const {
        if (a.width() != b.width()) {
            return a.width() < b.width();
        }
        if (a.height() != b.height()) {
            return a.height() < b.height();
        }
        for (auto c : a.coords()) {
            auto aa = static_cast<int>(a[c].type);
            auto bb = static_cast<int>(b[c].type);
            if (aa != bb) {
                return aa < bb;
            }
        }
        return false;
    }
};
}

namespace mapgen {

std::vector<Map> parseRooms(const std::string & roomsFileName) {
    namespace x3 = boost::spirit::x3;

    std::ifstream inFile(roomsFileName);
    if (!inFile) {
        throw mg_util::mg_error("couldn't open file to parse rooms");
    }

    using FileInIterator = std::istreambuf_iterator<char>;
    FileInIterator fileIt(inFile);

    using MultiPassIterator = boost::spirit::multi_pass<FileInIterator>;
    MultiPassIterator mpFirst(boost::spirit::make_default_multi_pass(fileIt));
    MultiPassIterator mpLast;

    using PositionIterator = boost::spirit::classic::position_iterator2<MultiPassIterator>;
    PositionIterator posFirst(mpFirst, mpLast, roomsFileName);
    PositionIterator posLast;

    std::vector<Map> rooms;
    auto finishMapAction = [&rooms](auto & ctx) {
    //auto finishMapAction = [&rooms](std::vector<std::vector<char>> & roomStr) {
        std::vector<std::string> roomStr = x3::_attr(ctx);
        if (roomStr.empty()) {
            throw mg_util::mg_error("empty map while parsing");
        }
        Map::SizeType width = roomStr.front().size();
        Map::SizeType height = roomStr.size();
        if (width == 0 || height == 0) {
            throw mg_util::mg_error("invalid map width/height while parsing");
        }
        for (const auto & m : roomStr) {
            if (static_cast<decltype(width)>(m.size()) != width) {
                throw mg_util::mg_error("width not constant");
            }
        }
        Map toAdd(width, height);

        for (auto c : toAdd.coords()) {
            toAdd[c] = char_to_MapTileType(roomStr[c.y][c.x]);
        }

        rooms.emplace_back(std::move(toAdd));
    };
    auto mapChar = x3::char_("#.+~?");

    auto oneMap = (+((+mapChar) >> x3::eol))[finishMapAction];

    auto parser = *(oneMap | x3::eol);

    bool success = x3::parse(posFirst, posLast, parser) && posFirst == posLast;

    if (!success) {
        std::stringstream ss;
        auto failPos = posFirst.get_position();
        ss << "bad parse in " << failPos.file << " at " << failPos.line << ":" << failPos.column << "\n";
        ss << posFirst.get_currentline() << '\n';
        for (int32_t i = 1; i < failPos.column; ++i) {
            ss << ' ';
        }
        ss << "^ around here\n";
        throw mg_util::mg_error(ss.str());
    }

    return rooms;
}

PrefabCombiner::PrefabCombiner(mg_util::Random & r, std::string roomsFileName): MapGenerator(r), roomsFileName_(std::move(roomsFileName)) {}

static bool isConnectionType(MapTileType type) {
    return type == MapTileType::Connection || type == MapTileType::Door;
}

static std::vector<Coord2D> getConnections(const Map & map) {
    std::vector<Coord2D> connections;
    auto range = map.coords_values();
    std::for_each(range.begin(), range.end(), [&connections](auto && cv) {
        auto type = cv.value.type;
        if (isConnectionType(type)) {
            connections.push_back(cv.coord);
        }
    });
    return connections;
}

static void addRoomRotationsFlips(const Map & room, std::set<Map> & newRooms) {
    newRooms.insert(room);
    Map cur = room.rotated();
    newRooms.insert(cur);
    cur = cur.rotated();
    newRooms.insert(cur);
    cur = cur.rotated();
    newRooms.insert(cur);
    cur = cur.flippedX();
    newRooms.insert(cur);
    cur = cur.rotated();
    newRooms.insert(cur);
    cur = cur.rotated();
    newRooms.insert(cur);
    cur = cur.rotated();
    newRooms.insert(cur);
}

static bool roomFitsInMap(const Map & room, Coord2D place, const Map & map) {
    if (!place.in_bounds(map.width(), map.height())) {
        return false;
    }
    return room.width() + place.x <= map.width() && room.height() + place.y <= map.height();
}

template<typename K, typename V, typename F>
static void remove_if_value(std::map<K, V> & m, F f) {
    auto cur = m.begin(), end = m.end();
    while (cur != end) {
        if (f(cur->second)) {
            cur = m.erase(cur);
        } else {
            ++cur;
        }
    }
}

static std::vector<Map> getAllRoomsRotationsFlips(const std::vector<Map> & rooms, const Map & map) {
    std::set<Map> newRooms;
    std::for_each(rooms.begin(), rooms.end(), [&newRooms](const Map & room) {
        addRoomRotationsFlips(room, newRooms);
    });
    std::vector<Map> allRooms(newRooms.begin(), newRooms.end());

    // remove rooms that don't fit the map
    {
        auto find = std::remove_if(allRooms.begin(), allRooms.end(), [&map](const Map & room) { return room.width() > map.width() || room.height() > map.height(); });
        allRooms.erase(find, allRooms.end());
    }
    return allRooms;
}

static std::vector<std::pair<Map, std::vector<Coord2D>>> getRoomsAndConnections(const std::vector<Map> & rooms) {
    std::vector<std::pair<Map, std::vector<Coord2D>>> roomsAndConnections;
    std::for_each(rooms.begin(), rooms.end(), [&roomsAndConnections](const Map & room) {
        roomsAndConnections.emplace_back(room, getConnections(room));
    });
    return roomsAndConnections;
}

/*
struct pair_hash {
    template<typename T, typename U>
    std::size_t operator()(const std::pair<T, U> & pair) {
        std::size_t hash = 0;
        boost::hash_combine(hash, pair.first);
        boost::hash_combine(hash, pair.second);
        return hash;
    }
};
*/

static void addRoomIntoMap(const Map & room, Coord2D placement, Map & map, std::vector<Coord2D> & mapConnections) {
    for (auto roomCoord : room.coords()) {
        auto mapCoord = roomCoord + placement;
        const auto roomType = room[roomCoord].type;
        auto & mapType = map[mapCoord].type;
        auto oldMapType = mapType;

        if (mapType == MapTileType::None) { // use room's tile type if map's tile type is None
            mapType = roomType;
        } else if (mapType == MapTileType::Connection && roomType == MapTileType::Door) {
            mapType = MapTileType::Door;
        }
        if (isConnectionType(oldMapType)) {
            // remove connection from list if we are using it now to connect room and map
            auto find = std::find(mapConnections.begin(), mapConnections.end(), mapCoord);
            if (find != mapConnections.end()) {
                mapConnections.erase(find);
            }
        } else if (isConnectionType(roomType)) {
            // if this is a new connection add it to list
            mapConnections.push_back(mapCoord);
        }
    }
}

static bool tileIsWalkable(MapTileType t) {
    return t == MapTileType::Ground || t == MapTileType::Door;
}

static void addIfPosIsWalkable(const Map & map, Coord2D pos, int32_t & count) {
    if (map.in_bounds(pos)) {
        if (tileIsWalkable(map[pos].type)) {
            ++count;
        }
    }
}

static int32_t numWalkableNeighbors(const Map & map, Coord2D pos) {
    int32_t count = 0;
    addIfPosIsWalkable(map, pos + Coord2D{0, -1}, count);
    addIfPosIsWalkable(map, pos + Coord2D{0, 1}, count);
    addIfPosIsWalkable(map, pos + Coord2D{1, 0}, count);
    addIfPosIsWalkable(map, pos + Coord2D{-1, 0}, count);
    return count;
}

static bool isValidWalkable(const Map & map, Coord2D pos) {
    return map.in_bounds(pos) && tileIsWalkable(map[pos].type);
}

static Coord2D findWalkableNeighbor(const Map & map, Coord2D pos) {
    auto test = pos + Coord2D{0, -1};
    if (isValidWalkable(map, test)) {
        return test;
    }
    test = pos + Coord2D{0, 1};
    if (isValidWalkable(map, test)) {
        return test;
    }
    test = pos + Coord2D{1, 0};
    if (isValidWalkable(map, test)) {
        return test;
    }
    test = pos + Coord2D{-1, 0};
    if (isValidWalkable(map, test)) {
        return test;
    }
    return pos;
}

static void removeDeadends(Map & map) {
    for (auto coord : map.coords()) {
        while (tileIsWalkable(map[coord].type) && numWalkableNeighbors(map, coord) <= 1) {
            map[coord] = MapTileType::Wall;
            coord = findWalkableNeighbor(map, coord);
        }
    }
}

void PrefabCombiner::modify_map(Map & map) {
    // read rooms from file
    const auto parsedRooms = parseRooms(roomsFileName_);
    if (parsedRooms.empty()) {
        return;
    }

    mg_util::Array2D<const Map *> mapRoomSources(map.width(), map.height());


    // get all possible rooms
    const auto rooms = getAllRoomsRotationsFlips(parsedRooms, map);
    // get all connection points for each room
    const auto roomsAndConnections = getRoomsAndConnections(rooms);

    // all existing connection points in map
    std::vector<Coord2D> mapConnections;

    // add random room to middle of map
    {
        auto randInt = rand_.rand_int_exc(rooms.size());
        const auto & room = rooms[randInt];
        auto mapMid = Coord2D(map.width() / 2, map.height() / 2);
        auto roomMid = Coord2D(room.width() / 2, room.height() / 2);
        addRoomIntoMap(room, mapMid - roomMid, map, mapConnections);
    }

    bool done = false;
    while (!done) {
        done = true;

        // for each (room, placement coord) pair, the number of connections between that placement of a room and the existing map
        std::map<std::pair<const Map *, Coord2D>, int32_t> numConnectionsForPlacement;
        for (const auto & roomConnections : roomsAndConnections) { // loop through all possible rooms
            const auto & room = roomConnections.first;

            for (const auto & roomConnection : roomConnections.second) { // loop through all connection points in current room
                for (const auto & mapConnection : mapConnections) { // loop through all connection points in map
                    Coord2D placement = mapConnection - roomConnection;
                    // room must be able to be placed in map at placement location
                    if (!roomFitsInMap(room, placement, map)) {
                        continue;
                    }
                    // number of connections between existing map and current room at the placement coord
                    int32_t connectionCount = 0;
                    // whether this placement is valid (tiles overlap)
                    bool valid = true;
                    for (auto roomCoord : room.coords()) { // check each coord in the room
                        auto mapCoord = roomCoord + placement; // corresponding coord in map
                        MapTileType r = room[roomCoord].type; // current tile type of room
                        MapTileType m = map[mapCoord].type; // current tile type of map
                        bool rIsConnection = isConnectionType(r); // current room tile is connection point
                        bool mIsConnection = isConnectionType(m); // current map tile is connection point
                        if (r == MapTileType::None || m == MapTileType::None) { // if either tile is None, okay
                            continue;
                        } else if (rIsConnection && mIsConnection) { // if both are connections, add connection count
                            ++connectionCount;
                        } else if (r == MapTileType::Wall && m == MapTileType::Wall) { // walls can overlap
                            continue;
                        } else { // all other combinations are invalid
                            valid = false;
                            break;
                        }
                    }

                    if (valid && connectionCount != 0) { // if there are connections, add to list
                        decltype(numConnectionsForPlacement)::key_type key{&room, placement};
                        numConnectionsForPlacement[key] = connectionCount;
                    }
                }
            }
            // get max connection count for all room/placement pairs
            auto maxCount = std::max_element(numConnectionsForPlacement.begin(), numConnectionsForPlacement.end(), [](const auto & a, const auto & b) { return a.second < b.second; })->second;
            // remove all pairs that don't have max count
            remove_if_value(numConnectionsForPlacement, [maxCount](auto count) { return count != maxCount; });
        }
        // if there are no values in list, quit
        done = numConnectionsForPlacement.empty();
        if (!done) {
            // choose room/coord randomly from list
            auto randInt = rand_.rand_int_exc(numConnectionsForPlacement.size());
            auto randRoomPlacement = numConnectionsForPlacement.begin();
            for (int32_t i = 0; i < randInt; ++i) {
                ++randRoomPlacement;
            }

            addRoomIntoMap(*randRoomPlacement->first.first, randRoomPlacement->first.second, map, mapConnections);
        }
    }

    for (auto & v : map.values()) {
        if (v.type == MapTileType::None) {
            v.type = MapTileType::Wall;
        } else if (v.type == MapTileType::Connection) {
            v.type = MapTileType::Ground;
        }
    }

    removeDeadends(map);
}

}
