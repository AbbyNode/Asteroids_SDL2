#include <stdio.h>
#include <string>
#include <unordered_map>

#include <SDL.h>
#include <SDL_image.h>

#include "gameobject.h"
#include "playership.h"
#include "texturewrapper.h"

namespace Asteroids {
	using std::string;
	using std::unordered_map;

#pragma region variables

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const char* TITLE = "Asteroids";

	const string RESOURCES_DIR = "resources/";
	const string RESOURCES_EXT = ".png";

	SDL_Window* window = NULL;

	int ticksPerSecond = 60;
	Uint32 tickDelay = 1000 / ticksPerSecond; // delay in millis between ticks
	Uint32 timeLastTick = 0;
	Uint32 timeNextTick = 0;

	enum class TextureName {
		SHIP, BULLET, ASTEROID1, ASTEROID2, ASTEROID3
	};
	unordered_map<TextureName, TextureWrapper*> textures;
	
	// TODO: Array of gameobjects?
	PlayerShip* ship = NULL;

#pragma endregion variables

#pragma region forward declare

	bool init();
	void close();

	bool loadTextures();
	SDL_Texture* loadTextureFromFile(string path);
	bool createShip();

	bool handleKeyboardEvent(SDL_Event const& e);

	void err(string msg);

#pragma endregion forward declare

#pragma region functions

	bool init() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
			err("SDL Init");
			return false;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			err("Window Create");
			return false;
		}

		TextureWrapper::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (TextureWrapper::renderer == NULL) {
			err("Renderer Create");
			return false;
		}

		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			err("SDL Img Init");
			return false;
		}

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

	//

	bool loadTextures() {
		// Load texture into hashmap
		auto loadHelper = [](string fileName, TextureName name) {
			// Create path
			string path = RESOURCES_DIR + fileName + RESOURCES_EXT;

			// Load texture
			SDL_Texture* texture = loadTextureFromFile(path);
			if (texture == NULL) {
				return false;
			}

			// Store texture into hashmap
			static TextureWrapper textureWrapper(texture, NULL);
			textures[name] = &textureWrapper;
			return true;
		};

		bool success = true;

		if (!loadHelper("ship", TextureName::SHIP)) {
			success = false;
		}
		if (!loadHelper("bullet", TextureName::BULLET)) {
			success = false;
		}
		if (!loadHelper("asteroid1", TextureName::ASTEROID1)) {
			success = false;
		}
		if (!loadHelper("asteroid2", TextureName::ASTEROID2)) {
			success = false;
		}
		if (!loadHelper("asteroid3", TextureName::ASTEROID3)) {
			success = false;
		}

		return success;
	}

	SDL_Texture* loadTextureFromFile(string path) {
		SDL_Texture* texture = NULL;

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) {
			err("Img load");
		}
		else {
			texture = SDL_CreateTextureFromSurface(TextureWrapper::renderer, loadedSurface);
			if (texture == NULL) {
				err("Texture Create");
			}

			SDL_FreeSurface(loadedSurface);
		}

		return texture;
	}

	bool createShip() {
		bool success = true;

		static PlayerShip shipObj(textures[TextureName::SHIP]);
		ship = &shipObj;

		return success;
	}

	//

	bool handleKeyboardEvent(SDL_Event const& e) {
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
			bool toggle = (e.type == SDL_KEYDOWN);

			switch (e.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				//ship->addVelocity(0, -0.05);
				ship->accelerate(toggle);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				//ship->addVelocity(0, 0.1);
				ship->decelerate(toggle);
				break;
			case SDLK_LEFT:
			case SDLK_a:
				ship->turn(toggle, false);
				//ship->addVelocity(0.1, 0);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				ship->turn(toggle, true);
				//ship->addVelocity(0.1, 0);
				break;
			}

			return true;
		}

		return false;
	}

	//

	void err(string msg) {
		printf("Error | %s\n%s", msg.c_str(), SDL_GetError());
		//printf("Error | %s\n%s\n%s", msg.c_str(), SDL_GetError(), IMG_GetError());
	}

#pragma endregion functions
}

int main(int argc, char* args[]) {
	using namespace Asteroids;

	if (init() && loadTextures()) {
		GameObject::SCREEN_HEIGHT = SCREEN_HEIGHT;
		GameObject::SCREEN_WIDTH = SCREEN_WIDTH;

		createShip();

		// TODO: Init game objects?

		SDL_Event e;

		bool quit = false;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else {
					handleKeyboardEvent(e);
				}
			}

			Uint32 timeTickNow = SDL_GetTicks();
			//printf("%u | %u | %u\n", ticks, tickTimeNext, tickDelay);
			if (timeTickNow >= timeNextTick) {
				Uint32 timeDelta = timeTickNow - timeLastTick;

				ship->tick(timeDelta);

				timeLastTick = timeTickNow;
				timeNextTick = timeTickNow + tickDelay;
			}

			SDL_SetRenderDrawColor(TextureWrapper::renderer, 0x00, 0x00, 0x00, 0xff);
			SDL_RenderClear(TextureWrapper::renderer);

			ship->render();

			SDL_RenderPresent(TextureWrapper::renderer);
		}
	}

	close();

	return 0;
}
