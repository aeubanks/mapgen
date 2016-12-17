////
//// Created by Arthur Eubanks on 13/1/16.
////
//
//#include "SDLButton.hpp"
//
// namespace sdl2 {
//	SDLButton::SDLButton(SDLRendererWrapper & renderer, string file_name,
//std::array<SDL_Rect, 4> rects) : texture_(renderer,
//SDLSurfaceWrapper(file_name)), state_(SDLButtonState::Out), rects_(rects) { }
//
//	SDL_Rect SDLButton::texture_rect() {
//		return rects_[static_cast<int>(state_)];
//	}
//
//	void SDLButton::handle_event(SDL_Event & event) {
//		if (event.type == SDL_MOUSEMOTION || event.type ==
//SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
//			int x, y;
//			SDL_GetMouseState(&x, &y);
//
//			//Check if mouse is in button
//			bool inside = true;
//
//			//Mouse is left of the button
//			if (x < mPosition.x) {
//				inside = false;
//			}
//				//Mouse is right of the button
//			else if (x > mPosition.x + BUTTON_WIDTH) {
//				inside = false;
//			}
//				//Mouse above the button
//			else if (y < mPosition.y) {
//				inside = false;
//			}
//				//Mouse below the button
//			else if (y > mPosition.y + BUTTON_HEIGHT) {
//				inside = false;
//			}
//
//			//Mouse is outside button
//			if (!inside) {
//				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
//			}
//				//Mouse is inside button
//			else {
//				//Set mouse over sprite
//				switch (event.type) {
//					case SDL_MOUSEMOTION:
//						mCurrentSprite =
//BUTTON_SPRITE_MOUSE_OVER_MOTION;
//						break;
//
//					case SDL_MOUSEBUTTONDOWN:
//						mCurrentSprite =
//BUTTON_SPRITE_MOUSE_DOWN;
//						break;
//
//					case SDL_MOUSEBUTTONUP:
//						mCurrentSprite =
//BUTTON_SPRITE_MOUSE_UP;
//						break;
//				}
//			}
//		}
//	}
//
//	SDLTextureWrapper & SDLButton::texture() {
//		return texture_;
//	}
//}
