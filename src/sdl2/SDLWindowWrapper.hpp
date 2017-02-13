#pragma once

#include "sdl2_include.hpp"

#include <memory>

namespace sdl2 {
class SDLWindowWrapper {
  public:
    // create a window with the given parameters
    SDLWindowWrapper(std::string title, int32_t width, int32_t height);
    SDLWindowWrapper(std::string title, int32_t x, int32_t y, int32_t width, int32_t height,
                     Uint32 flags);

    // the width/height of the window
    int32_t width();
    int32_t height();

    // the underlying SDL_Window *
    SDL_Window * get();

  private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
};
}

