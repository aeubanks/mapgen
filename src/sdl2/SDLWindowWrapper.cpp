//
// Created by Arthur Eubanks on 9/1/16.
//

#include "SDLWindowWrapper.hpp"

namespace sdl2 {
SDLWindowWrapper::SDLWindowWrapper(std::string title, int32_t width, int32_t height)
    : SDLWindowWrapper(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, SDL_WINDOW_SHOWN) {}

SDLWindowWrapper::SDLWindowWrapper(std::string title, int32_t x, int32_t y, int32_t width,
                                   int32_t height, Uint32 flags)
    : window_(SDL_CreateWindow(title.c_str(), x, y, width, height, flags),
              SDL_DestroyWindow) {
    if (!window_) {
        throw sdl2_error("SDL_CreateWindow");
    }
}

int32_t SDLWindowWrapper::width() {
    int32_t w;
    SDL_GetWindowSize(get(), &w, NULL);
    return w;
}

int32_t SDLWindowWrapper::height() {
    int32_t h;
    SDL_GetWindowSize(get(), NULL, &h);
    return h;
}

SDL_Window * SDLWindowWrapper::get() { return window_.get(); }
}
