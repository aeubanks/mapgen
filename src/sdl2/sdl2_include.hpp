#pragma once

#include "SDL2/SDL.h"

#include <stdexcept>

namespace sdl2 {

class sdl2_error : public std::runtime_error {
  public:
    //		sdl2_error(std::string err);
    sdl2_error(std::string err, const char * (*get_error)() = SDL_GetError);
};
}

