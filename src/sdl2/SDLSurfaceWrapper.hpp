//
// Created by Arthur Eubanks on 8/1/16.
//

#ifndef MAPGEN_SDLSURFACEWRAPPER_HPP
#define MAPGEN_SDLSURFACEWRAPPER_HPP

#include "sdl2_global.hpp"

#include "mapgen/Map.hpp"

#include <memory>
#include <vector>

namespace sdl2 {
class SDLSurfaceWrapper {
  public:
    //		static void fill_rect(SDL_Surface * surface, Uint8 r, Uint8 g, Uint8
    //b);
    //		static void fill_rect(SDL_Surface * surface, Uint8 r, Uint8 g, Uint8
    //b, SDL_Rect rect);
    //		static void fill_rect(SDL_Surface * surface, Uint32 color);
    //		static void fill_rect(SDL_Surface * surface, Uint32 color, SDL_Rect
    //rect);

    SDLSurfaceWrapper(std::vector<unsigned char> & pixels, int width, int height);
    SDLSurfaceWrapper(std::vector<unsigned char> && pixels, int width, int height);
    // create a surface from a map
    SDLSurfaceWrapper(mapgen::Map & map, int tile_size = 1);
    SDLSurfaceWrapper(SDL_Surface * surface);
    SDLSurfaceWrapper(string file_name);

    // the underlying SDL_Surface *
    SDL_Surface * get();

    // the width/height of the surface
    int width();
    int height();

    // set/get the color key
    void set_color_key(Uint8 r, Uint8 g, Uint8 b);
    void unset_color_key();
    Uint32 get_color_key();

    //		void blit_onto(SDL_Surface * other);
    //		void blit_onto(SDL_Rect src_rect, SDL_Surface * other);
    //		void blit_onto(SDL_Surface * other, SDL_Rect dest_rect);
    //		void blit_onto(SDL_Rect src_rect, SDL_Surface * other, SDL_Rect
    //dest_rect);
    //		void blit_onto(SDLSurfaceWrapper & other);
    //		void blit_onto(SDL_Rect src_rect, SDLSurfaceWrapper & other);
    //		void blit_onto(SDLSurfaceWrapper & other, SDL_Rect dest_rect);
    //		void blit_onto(SDL_Rect src_rect, SDLSurfaceWrapper & other,
    //SDL_Rect dest_rect);
    //
    //		void fill_rect(Uint32 color);
    //		void fill_rect(Uint32 color, SDL_Rect rect);

  private:
    // the pixels for the surface
    std::vector<unsigned char> pixels_;

    // the surface, freed in destructor
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface_;
};
}

#endif // MAPGEN_SDLSURFACEWRAPPER_HPP
