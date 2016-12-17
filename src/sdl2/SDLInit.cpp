#include "sdl2/SDLInit.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "fmt/fmt_println.hpp"

namespace sdl2 {

SDL2Init::SDL2Init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) <
        0) {
        throw sdl2::sdl2_error("SDL_Init");
    }
}

SDL2Init::~SDL2Init() {
    SDL_Quit();
}

SDL2TTFInit::SDL2TTFInit() {
    int img_init = IMG_INIT_PNG;
    auto not_init = IMG_Init(img_init) ^ img_init;
    if (not_init != 0) {
        fmt::MemoryWriter mw;
        mw << fmt::bin(not_init);
        throw sdl2::sdl2_error(
            fmt::format("IMG_Init failed on flags {}", mw.c_str()), IMG_GetError);
    }
}

SDL2TTFInit::~SDL2TTFInit() {
    TTF_Quit();
}

SDL2ImageInit::SDL2ImageInit() {
    if (TTF_Init() == -1) {
        throw sdl2::sdl2_error("TTF_Init", TTF_GetError);
    }
}

SDL2ImageInit::~SDL2ImageInit() {
    IMG_Quit();
}

}
