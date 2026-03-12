#pragma once
#include "input/SDLInput.hpp"
#include "window/SDLWindow_Win32.hpp"

namespace aZero::SDLCppWrapper {
	inline void Init(SDL_InitFlags flags = SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) {
		SDL_Init(flags);
	}

	inline void Shutdown() {
		SDL_Quit();
	}
}