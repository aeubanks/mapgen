//
// Created by Arthur Eubanks on 10/1/16.
//

#ifndef MAPGEN_SDL2_GLOBAL_HPP
#define MAPGEN_SDL2_GLOBAL_HPP

#include "SDL2/SDL.h"

#include <stdexcept>
#include <string>

namespace sdl2 {
	using std::string;

	class sdl2_error : public std::runtime_error {
	public:
//		sdl2_error(std::string err);
		sdl2_error(std::string err, const char * (*get_error)() = SDL_GetError);
	};
}


#endif //MAPGEN_SDL2_GLOBAL_HPP
