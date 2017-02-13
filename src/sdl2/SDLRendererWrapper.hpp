#pragma once

#include "sdl2_include.hpp"

#include "SDLTextureWrapper.hpp"
#include "SDLWindowWrapper.hpp"

#include <memory>

namespace sdl2 {
class SDLTextureWrapper;

class SDLRendererWrapper {
  public:
    // create a renderer from a window
    //		SDLRendererWrapper(SDL_Window * window, Uint32 flags =
    //SDL_RENDERER_ACCELERATED);
    SDLRendererWrapper(SDLWindowWrapper & window,
                       Uint32 flags = SDL_RENDERER_ACCELERATED);

    // set the draw color for fill, draw_*, etc
    void set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF);
    // clear the screen, filling it with the draw color
    void clear();
    // refresh the screen and display what is being drawn
    void present();

    // copy a texture onto the screen, optionally specifying the src and dest_rect
    void copy(SDLTextureWrapper & texture);
    void copy(SDL_Rect dest_rect, SDLTextureWrapper & texture);
    void copy(SDLTextureWrapper & texture, SDL_Rect texture_rect);
    void copy(SDL_Rect dest_rect, SDLTextureWrapper & texture,
              SDL_Rect texture_rect);

    void copy(SDL_Rect dest_rect, SDLTextureWrapper & texture,
              SDL_Rect texture_rect, double angle,
              SDL_RendererFlip flip = SDL_FLIP_NONE);
    void copy(SDL_Rect dest_rect, SDLTextureWrapper & texture,
              SDL_Rect texture_rect, double angle, SDL_Point center,
              SDL_RendererFlip flip = SDL_FLIP_NONE);

    // fill the whole screen with the draw color
    void fill();
    // draw a rect using the draw color
    void draw_rect(SDL_Rect rect);
    // draw a line using the draw color
    void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
    // draw a point using the draw color
    void draw_point(int32_t x, int32_t y);

    // get the current viewport
    SDL_Rect get_viewport();
    // set the viewport
    void set_viewport(SDL_Rect viewport);
    // reset the viewport so it is the whole window
    void reset_viewport();

    SDL_Renderer * get();

  private:
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer_;
};
}

