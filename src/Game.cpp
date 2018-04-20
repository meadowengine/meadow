#ifdef __EMSCRIPTEN__
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <iostream>

#include "Game.h"

#ifdef __EMSCRIPTEN__
static SDL_Surface *surface;
#else
static SDL_Window *win;
static SDL_GLContext glcontext;
#endif

namespace Meadow {
	extern void tickEntities(float dt);
	extern void renderEntities();
}

extern "C" int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "Meadow: Could not initialize SDL: " << SDL_GetError() << std::endl;
		return 1;
	}

	Meadow::Game theGame;

	while (argc) {
		theGame.args.push_back(argv[0]);
		--argc;
	}

	setup(theGame);

#ifdef __EMSCRIPTEN__
#else
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	win = SDL_CreateWindow(
		theGame.title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		theGame.width, theGame.height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if (!win) {
		std::cerr << "Meadow: Could not create window: " << SDL_GetError() << std::endl;
		exit(2);
	}

	glcontext = SDL_GL_CreateContext(win);
	if (!glcontext) {
		SDL_DestroyWindow(win);
		std::cerr << "Meadow: Could not create OpenGL context: " << SDL_GetError() << std::endl;
		exit(3);
	}

	SDL_Event evt;

	auto previousTime = SDL_GetTicks();

	for (;;) {
		auto beginTime = SDL_GetTicks();

		if (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				goto quit;
			}
		}

		auto tickTime = SDL_GetTicks();
		Meadow::tickEntities((tickTime - previousTime) / 1000.f);
		previousTime = SDL_GetTicks();

		Meadow::renderEntities();

		SDL_GL_SwapWindow(win);

		auto endTime = SDL_GetTicks();
		endTime -= beginTime;

		if (endTime < 20) {
			SDL_Delay(20 - endTime);
		}
	}

	quit:

	SDL_DestroyWindow(win);
#endif

	SDL_Quit();

	return 0;
}