//
// Created by Arthur Eubanks on 10/1/16.
//

#include "SDLTextureWrapper.hpp"

namespace sdl2 {
//	SDLTextureWrapper::SDLTextureWrapper(SDL_Renderer * renderer,
//SDL_Surface * surface): texture_(SDL_CreateTextureFromSurface(renderer,
//surface)), width_(surface->w), height_(surface->h) {
//		if (!texture_) {
//			throw sdl2_error("SDL_CreateTextureFromSurface");
//		}
//	}

SDLTextureWrapper::SDLTextureWrapper(SDLRendererWrapper & renderer,
                                     SDLSurfaceWrapper & surface)
    : texture_(
          SDL_CreateTextureFromSurface(renderer.get(), surface.get()),
          SDL_DestroyTexture) /* SDLTextureWrapper(renderer.get(), surface.get())*/
{
    if (!texture_) {
        throw sdl2_error("SDL_CreateTextureFromSurface");
    }
}

SDLTextureWrapper::SDLTextureWrapper(SDLRendererWrapper & renderer,
                                     SDLSurfaceWrapper && surface)
    : SDLTextureWrapper(renderer, surface) {}

int SDLTextureWrapper::width() {
    int w;
    SDL_QueryTexture(get(), NULL, NULL, &w, NULL);
    return w;
}

int SDLTextureWrapper::height() {
    int h;
    SDL_QueryTexture(get(), NULL, NULL, NULL, &h);
    return h;
}

SDL_Texture * SDLTextureWrapper::get() { return texture_.get(); }

void SDLTextureWrapper::set_color_mod(Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetTextureColorMod(get(), r, g, b);
}

std::tuple<Uint8, Uint8, Uint8> SDLTextureWrapper::get_color_mod() {
    Uint8 r, g, b;
    SDL_GetTextureColorMod(get(), &r, &g, &b);
    return std::tuple<Uint8, Uint8, Uint8>{r, g, b};
}

void SDLTextureWrapper::unset_color_mod() { set_color_mod(0xFF, 0xFF, 0xFF); }

void SDLTextureWrapper::unset_alpha_mod() { set_alpha_mod(0xFF); }

void SDLTextureWrapper::set_alpha_mod(Uint8 a) {
    SDL_SetTextureAlphaMod(get(), a);
}

Uint8 SDLTextureWrapper::get_alpha_mod() {
    Uint8 a;
    SDL_GetTextureAlphaMod(get(), &a);
    return a;
}

void SDLTextureWrapper::set_blend_mode(SDL_BlendMode blend_mode) {
    SDL_SetTextureBlendMode(get(), blend_mode);
}
}
