//
// Created by Arthur Eubanks on 8/1/16.
//

#include "SDLSurfaceWrapper.hpp"

#include "lodepng/lodepng_map.hpp"

#include "SDL2/SDL_image.h"

namespace sdl2 {
//	void SDLSurfaceWrapper::fill_rect(SDL_Surface * surface, Uint8 r, Uint8 g, Uint8 b) {
//		fill_rect(surface, SDL_MapRGB(surface->format, r, g, b));
//	}
//
//	void SDLSurfaceWrapper::fill_rect(SDL_Surface * surface, Uint8 r, Uint8 g, Uint8 b, SDL_Rect rect) {
//		fill_rect(surface, SDL_MapRGB(surface->format, r, g, b), rect);
//	}
//
//	void SDLSurfaceWrapper::fill_rect(SDL_Surface * surface, Uint32 color) {
//		SDL_FillRect(surface, NULL, color);
//	}
//
//	void SDLSurfaceWrapper::fill_rect(SDL_Surface * surface, Uint32 color, SDL_Rect rect) {
//		SDL_FillRect(surface, &rect, color);
//	}


	SDLSurfaceWrapper::SDLSurfaceWrapper(SDL_Surface * surface): pixels_(), surface_(surface, SDL_FreeSurface) {
		if (!surface_) {
			throw sdl2_error("SDLSurfaceWrapper", NULL);
		}
	}

	SDLSurfaceWrapper::SDLSurfaceWrapper(std::vector<unsigned char> & pixels, int width, int height) : pixels_(pixels), surface_(SDL_CreateRGBSurfaceFrom(pixels_.data(), width, height, 32, 4 * width, 0, 0, 0, 0), SDL_FreeSurface) {
		if (!surface_) {
			throw sdl2_error("SDL_CreateRGBSurfaceFrom");
		}
	}

	SDLSurfaceWrapper::SDLSurfaceWrapper(std::vector<unsigned char> && pixels, int width, int height) : SDLSurfaceWrapper(pixels, width, height) { }

	SDLSurfaceWrapper::SDLSurfaceWrapper(mapgen::Map & map, int tile_size) : SDLSurfaceWrapper(lodepng::map_to_image(map, tile_size), map.width() * tile_size, map.height() * tile_size) { }

	SDLSurfaceWrapper::SDLSurfaceWrapper(string file_name) : pixels_(), surface_(IMG_Load(file_name.c_str()), SDL_FreeSurface) {
		if (!surface_) {
			throw sdl2_error("SDL_LoadBMP");
		}
	}

	SDL_Surface * SDLSurfaceWrapper::get() {
		return surface_.get();
	}

	int SDLSurfaceWrapper::width() {
		return get()->h;
	}

	int SDLSurfaceWrapper::height() {
		return get()->w;
	}

	void SDLSurfaceWrapper::set_color_key(Uint8 r, Uint8 g, Uint8 b) {
		SDL_SetColorKey(get(), SDL_TRUE, SDL_MapRGB(get()->format, r, g, b));
	}

	void SDLSurfaceWrapper::unset_color_key() {
		SDL_SetColorKey(get(), SDL_FALSE, 0);
	}

	Uint32 SDLSurfaceWrapper::get_color_key() {
		Uint32 key;
		SDL_GetColorKey(get(), &key);
		return key;
	}

//	void SDLSurfaceWrapper::fill_rect(Uint32 color) {
//		fill_rect(get(), color);
//	}
//
//	void SDLSurfaceWrapper::fill_rect(Uint32 color, SDL_Rect rect) {
//		fill_rect(get(), color, rect);
//	}
//
//	void SDLSurfaceWrapper::blit_onto(SDL_Surface * other) {
//		SDL_BlitSurface(get(), NULL, other, NULL);
//	}
//
//	void SDLSurfaceWrapper::blit_onto(SDL_Rect src_rect, SDL_Surface * other) {
//		SDL_BlitSurface(get(), &src_rect, other, NULL);
//	}
//
//	void SDLSurfaceWrapper::blit_onto(SDL_Surface * other, SDL_Rect dest_rect) {
//		SDL_BlitSurface(get(), NULL, other, &dest_rect);
//	}
//
//	void SDLSurfaceWrapper::blit_onto(SDL_Rect src_rect, SDL_Surface * other, SDL_Rect dest_rect) {
//		SDL_BlitSurface(get(), &src_rect, other, &dest_rect);
//	}
//
//	void SDLSurfaceWrapper::blit_onto(SDLSurfaceWrapper & other) {
//		blit_onto(other.get());
//	}
//
//	void SDLSurfaceWrapper::blit_onto(SDL_Rect src_rect, SDLSurfaceWrapper & other) {
//		blit_onto(src_rect, other.get());
//	}
//
//	void SDLSurfaceWrapper::blit_onto(SDLSurfaceWrapper & other, SDL_Rect dest_rect) {
//		blit_onto(other.get(), dest_rect);
//	}
//
//	void SDLSurfaceWrapper::blit_onto(SDL_Rect src_rect, SDLSurfaceWrapper & other, SDL_Rect dest_rect) {
//		blit_onto(src_rect, other.get(), dest_rect);
//	}
}
