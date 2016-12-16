////
//// Created by Arthur Eubanks on 13/1/16.
////
//
//#ifndef MAPGEN_SDLBUTTON_HPP
//#define MAPGEN_SDLBUTTON_HPP
//
//#include "sdl2_global.hpp"
//
//#include "SDLTextureWrapper.hpp"
//
//#include <array>
//
//namespace sdl2 {
//	class SDLButton {
//	public:
//		SDLButton(SDLRendererWrapper & renderer, string file_name, std::array<SDL_Rect, 4> rects);
//
//		void handle_event(SDL_Event & event);
//
//		SDLTextureWrapper & texture();
//		SDL_Rect texture_rect();
//
//	private:
//		enum class SDLButtonState {
//			Out,
//			Over,
//			Down,
//			Up,
//		};
//
//		SDLTextureWrapper texture_;
//		SDLButtonState state_;
//		std::array<SDL_Rect, 4> rects_;
//	};
//}
//
//
//#endif //MAPGEN_SDLBUTTON_HPP
