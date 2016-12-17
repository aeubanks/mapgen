//
// Created by Arthur Eubanks on 9/1/16.
//

#ifndef MAPGEN_SDLWINDOWWRAPPER_HPP
#define MAPGEN_SDLWINDOWWRAPPER_HPP

#include "sdl2_global.hpp"

#include <memory>

namespace sdl2 {
class SDLWindowWrapper {
  public:
    // create a window with the given parameters
    SDLWindowWrapper(std::string title, int width, int height);
    SDLWindowWrapper(std::string title, int x, int y, int width, int height,
                     Uint32 flags);

    // the width/height of the window
    int width();
    int height();

    // the underlying SDL_Window *
    SDL_Window * get();

  private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
};
}

#endif // MAPGEN_SDLWINDOWWRAPPER_HPP
