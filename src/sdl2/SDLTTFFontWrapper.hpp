//
// Created by Arthur Eubanks on 12/1/16.
//

#ifndef MAPGEN_SDLTTFFONTWRAPPER_HPP
#define MAPGEN_SDLTTFFONTWRAPPER_HPP

#include "sdl2_global.hpp"

#include "SDL2/SDL_ttf.h"

#include "SDLRendererWrapper.hpp"

#include <memory>

namespace sdl2 {
	class SDLTTFFontWrapper {
	public:
		SDLTTFFontWrapper(string file_name, int size);
		SDLTextureWrapper render_font_solid(SDLRendererWrapper & renderer, string str, SDL_Color color);

		TTF_Font * get();

	private:
		std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font_;
	};
}


#endif //MAPGEN_SDLTTFFONTWRAPPER_HPP
