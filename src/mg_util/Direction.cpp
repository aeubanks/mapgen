#include "Direction.hpp"

namespace mg_util {

Direction Direction::Up(Direction::DirectionEnum::Up);
Direction Direction::Down(Direction::DirectionEnum::Down);
Direction Direction::Left(Direction::DirectionEnum::Left);
Direction Direction::Right(Direction::DirectionEnum::Right);

Direction Direction::cw() {
    switch (this->dir) {
        case DirectionEnum::Up:
            return Right;
        case DirectionEnum::Down:
            return Left;
        case DirectionEnum::Left:
            return Up;
        case DirectionEnum::Right:
            return Down;
    }
}

Direction Direction::ccw() {
    switch (this->dir) {
        case DirectionEnum::Up:
            return Left;
        case DirectionEnum::Down:
            return Right;
        case DirectionEnum::Left:
            return Down;
        case DirectionEnum::Right:
            return Up;
    }
}

Coord2D Direction::coord() {
    switch (this->dir) {
        case DirectionEnum::Up:
            return {0, 1};
        case DirectionEnum::Down:
            return {0, -1};
        case DirectionEnum::Left:
            return {-1, 0};
        case DirectionEnum::Right:
            return {1, 0};
    }
}
}

mg_util::Coord2D & operator+=(mg_util::Coord2D & coord, mg_util::Direction dir) {
    return coord += dir.coord();
}

mg_util::Coord2D operator+(mg_util::Coord2D coord, mg_util::Direction dir) {
    return coord += dir;
}

mg_util::Coord2D operator+(mg_util::Direction dir, mg_util::Coord2D coord) {
    return coord += dir;
}
