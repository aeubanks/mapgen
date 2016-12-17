//
// Created by Arthur Eubanks on 10/1/16.
//

#include "sdl2_global.hpp"

#include "fmt/format.hpp"

namespace sdl2 {
//	sdl2_error::sdl2_error(std::string err) :
//std::runtime_error(fmt::format("{} error: {}", err, SDL_GetError())) { }
sdl2_error::sdl2_error(std::string err, const char * (*get_error)())
    : std::runtime_error(get_error == NULL
                             ? err
                             : fmt::format("{} error: {}", err, get_error())) {}
}
