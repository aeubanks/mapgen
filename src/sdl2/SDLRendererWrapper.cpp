//
// Created by Arthur Eubanks on 10/1/16.
//

#include "SDLRendererWrapper.hpp"

namespace sdl2 {
//	SDLRendererWrapper::SDLRendererWrapper(SDL_Window * window, Uint32
//flags) : renderer_(SDL_CreateRenderer(window, -1, flags)) {
//		if (!renderer_) {
//			throw sdl2_error("SDL_CreateRenderer");
//		}
//	}
SDLRendererWrapper::SDLRendererWrapper(SDLWindowWrapper & window, Uint32 flags)
    : renderer_(SDL_CreateRenderer(window.get(), -1, flags),
                SDL_DestroyRenderer) {
    if (!renderer_) {
        throw sdl2_error("SDL_CreateRenderer");
    }
}

//	SDLRendererWrapper::SDLRendererWrapper(SDLWindowWrapper & window, Uint32
//flags) : SDLRendererWrapper(window.get(), flags) { }

void SDLRendererWrapper::present() { SDL_RenderPresent(get()); }

void SDLRendererWrapper::clear() { SDL_RenderClear(get()); }

void SDLRendererWrapper::copy(SDLTextureWrapper & texture) {
    SDL_RenderCopy(get(), texture.get(), NULL, NULL);
}

void SDLRendererWrapper::copy(SDL_Rect dest_rect, SDLTextureWrapper & texture) {
    SDL_RenderCopy(get(), texture.get(), NULL, &dest_rect);
}

void SDLRendererWrapper::copy(SDLTextureWrapper & texture,
                              SDL_Rect texture_rect) {
    SDL_RenderCopy(get(), texture.get(), &texture_rect, NULL);
}

void SDLRendererWrapper::copy(SDL_Rect dest_rect, SDLTextureWrapper & texture,
                              SDL_Rect texture_rect) {
    SDL_RenderCopy(get(), texture.get(), &texture_rect, &dest_rect);
}

void SDLRendererWrapper::draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    SDL_RenderDrawLine(get(), x1, y1, x2, y2);
}

void SDLRendererWrapper::draw_point(int32_t x, int32_t y) {
    SDL_RenderDrawPoint(get(), x, y);
}

void SDLRendererWrapper::fill() { SDL_RenderFillRect(get(), NULL); }

void SDLRendererWrapper::draw_rect(SDL_Rect rect) {
    SDL_RenderFillRect(get(), &rect);
}

SDL_Rect SDLRendererWrapper::get_viewport() {
    SDL_Rect rect;
    SDL_RenderGetViewport(get(), &rect);
    return rect;
}

void SDLRendererWrapper::set_viewport(SDL_Rect viewport) {
    SDL_RenderSetViewport(get(), &viewport);
}

void SDLRendererWrapper::reset_viewport() {
    SDL_RenderSetViewport(get(), NULL);
}

void SDLRendererWrapper::set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(get(), r, g, b, a);
}

SDL_Renderer * SDLRendererWrapper::get() { return renderer_.get(); }

void SDLRendererWrapper::copy(SDL_Rect dest_rect, SDLTextureWrapper & texture,
                              SDL_Rect texture_rect, double angle,
                              SDL_RendererFlip flip) {
    SDL_RenderCopyEx(get(), texture.get(), &texture_rect, &dest_rect, angle, NULL,
                     flip);
}

void SDLRendererWrapper::copy(SDL_Rect dest_rect, SDLTextureWrapper & texture,
                              SDL_Rect texture_rect, double angle,
                              SDL_Point center, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(get(), texture.get(), &texture_rect, &dest_rect, angle,
                     &center, flip);
}
}
