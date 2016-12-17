#ifndef SDL2_INIT_HPP
#define SDL2_INIT_HPP

#include "sdl2/sdl2_global.hpp"

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

#endif
