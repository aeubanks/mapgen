//
// Created by Arthur Eubanks on 12/1/16.
//

#include "SDLTTFFontWrapper.hpp"

#include "SDLSurfaceWrapper.hpp"

namespace sdl2 {
	SDLTTFFontWrapper::SDLTTFFontWrapper(string file_name, int size) : font_(TTF_OpenFont(file_name.c_str(), size), TTF_CloseFont) {
		if (!font_) {
			throw sdl2_error("TTF_OpenFont", TTF_GetError);
		}
	}

	SDLTextureWrapper SDLTTFFontWrapper::render_font_solid(SDLRendererWrapper & renderer, string str, SDL_Color color) {
		auto text = TTF_RenderText_Solid(get(), str.c_str(), color);
		if (!text) {
			throw sdl2_error("TTF_RenderText_Solid", TTF_GetError);
		}
		return SDLTextureWrapper(renderer, SDLSurfaceWrapper(text));
	}

	TTF_Font * SDLTTFFontWrapper::get() {
		return font_.get();
	}
}
