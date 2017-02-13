#pragma once

#include "sdl2/sdl2_include.hpp"

namespace sdl2 {
class SDL2Init {
public:
    SDL2Init();
    ~SDL2Init();
};

class SDL2TTFInit {
public:
    SDL2TTFInit();
    ~SDL2TTFInit();
};

class SDL2ImageInit {
public:
    SDL2ImageInit();
    ~SDL2ImageInit();
};
}

