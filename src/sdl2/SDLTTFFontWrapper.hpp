#pragma once

#include "sdl2_include.hpp"

#include "SDL2/SDL_ttf.h"

#include "SDLRendererWrapper.hpp"

#include <memory>

namespace sdl2 {
class SDLTTFFontWrapper {
  public:
    SDLTTFFontWrapper(std::string file_name, int32_t size);
    SDLTextureWrapper render_font_solid(SDLRendererWrapper & renderer, std::string str,
                                        SDL_Color color);

    TTF_Font * get();

  private:
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font_;
};
}

