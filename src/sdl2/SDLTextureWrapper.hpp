#pragma once

#include "sdl2_include.hpp"

#include "SDLRendererWrapper.hpp"
#include "SDLSurfaceWrapper.hpp"

#include <memory>

namespace sdl2 {
class SDLRendererWrapper;

class SDLTextureWrapper {
  public:
    // a texture copied from the surface using the renderer
    SDLTextureWrapper(SDLRendererWrapper & renderer, SDLSurfaceWrapper & surface);
    SDLTextureWrapper(SDLRendererWrapper & renderer, SDLSurfaceWrapper && surface);
    //		SDLTextureWrapper(SDL_Renderer * renderer, SDL_Surface *
    //surface);

    // width/height of texture
    int32_t width();
    int32_t height();

    // set/unset/get color modulation of texture
    void set_color_mod(Uint8 r, Uint8 g, Uint8 b);
    void unset_color_mod();
    std::tuple<Uint8, Uint8, Uint8> get_color_mod();

    // set alpha mode of texture
    void set_blend_mode(SDL_BlendMode blend_mode);

    // set/unset/get alpha modulation of texture
    void unset_alpha_mod();
    void set_alpha_mod(Uint8 a);
    Uint8 get_alpha_mod();

    // the underlying texture
    SDL_Texture * get();

  private:
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture_;
};
}

