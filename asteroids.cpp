#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "texturewrapper.h"
#include "gameobject.h"

namespace Asteroids {
	using std::string;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const char* TITLE = "Asteroids";

	SDL_Window* window;

	//

	bool init();
	bool loadTextures();
	void close();
	void err(string msg);

	//

	bool init() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
			err("SDL Init");
			return false;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			err("Window");
			return false;
		}

		TextureWrapper::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (TextureWrapper::renderer == NULL) {
			err("Renderer");
			return false;
		}

		SDL_SetRenderDrawColor(TextureWrapper::renderer, 0x00, 0x00, 0x00, 0xff);

		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			err("SDL Img Init");
			return false;
		}

		return true;
	}

	bool loadTextures() {
		return true;
	}

	void close() {
		// TODO: Destroy and deallocate all textures

		SDL_DestroyRenderer(TextureWrapper::renderer);
		TextureWrapper::renderer = NULL;

		SDL_DestroyWindow(window);
		window = NULL;

		IMG_Quit();
		SDL_Quit();
	}

	void err(string msg) {
		printf("Error | %s\n%s", msg.c_str(), SDL_GetError());
		//printf("Error | %s\n%s\n%s", msg.c_str(), SDL_GetError(), IMG_GetError());
	}

	//
}


int main(int argc, char* args[]) {
	using namespace Asteroids;

	if (init() && loadTextures()) {
		bool quit = false;

		SDL_Event e;

		//GameObject ship = GameObject(, 32, 32);

		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}

			// TODO: delta time

			// tick

			SDL_RenderClear(TextureWrapper::renderer);
			// render
			SDL_RenderPresent(TextureWrapper::renderer);
		}
	}

	close();

	return 0;
}