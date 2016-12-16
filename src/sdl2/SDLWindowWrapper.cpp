//
// Created by Arthur Eubanks on 9/1/16.
//

#include "SDLWindowWrapper.hpp"

#include "mg_util/mg_util_global.hpp"
#include "fmt/format.hpp"

namespace sdl2 {
	SDLWindowWrapper::SDLWindowWrapper(std::string title, int width, int height): SDLWindowWrapper(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN) { }

	SDLWindowWrapper::SDLWindowWrapper(std::string title, int x, int y, int width, int height, Uint32 flags): window_(SDL_CreateWindow(title.c_str(), x, y, width, height, flags), SDL_DestroyWindow) {
		if (!window_) {
			throw sdl2_error("SDL_CreateWindow");
		}
	}

	int SDLWindowWrapper::width() {
		int w;
		SDL_GetWindowSize(get(), &w, NULL);
		return w;
	}

	int SDLWindowWrapper::height() {
		int h;
		SDL_GetWindowSize(get(), NULL, &h);
		return h;
	}

	SDL_Window * SDLWindowWrapper::get() {
		return window_.get();
	}
}
