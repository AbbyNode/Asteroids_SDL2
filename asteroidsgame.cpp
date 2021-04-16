#include <cstdlib>
#include <stdio.h>
#include <string>
#include <unordered_map>

#include <SDL.h>
#include <SDL_image.h>

#include "asteroid.h"
#include "gameobject.h"
#include "playership.h"
#include "texturewrapper.h"

namespace AsteroidsGame {
	using std::string;
	using std::unordered_map;
	using std::vector;

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

	PlayerShip* playerShip = NULL;

	vector<Asteroid*> asteroids;

#pragma endregion variables

#pragma region forward declare

	bool init();
	void close();

	SDL_Texture* loadTextureFromFile(string path);
	bool loadTextures();
	bool createShip();

	void tick(float delta);
	void render();
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
		delete playerShip;
		asteroids.clear();
		textures.clear();

		SDL_DestroyRenderer(TextureWrapper::renderer);
		TextureWrapper::renderer = NULL;

		SDL_DestroyWindow(window);
		window = NULL;

		IMG_Quit();
		SDL_Quit();
	}

	//

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
			textures[name] = new TextureWrapper(texture, NULL);
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

	bool createShip() {
		playerShip = new PlayerShip(textures[TextureName::SHIP]);
		return (playerShip != NULL);
	}

	//

	void tick(float delta) {
		playerShip->tick(delta);

		for (Asteroid* asteroid : asteroids) {
			asteroid->tick(delta);
		}
	}

	void render() {
		playerShip->render();

		for (Asteroid* asteroid : asteroids) {
			asteroid->render();
		}
	}

	bool handleKeyboardEvent(SDL_Event const& e) {
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
			bool toggle = (e.type == SDL_KEYDOWN);

			switch (e.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				//playerShip->addVelocity(0, -0.05);
				playerShip->accelerate(toggle);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				//playerShip->addVelocity(0, 0.1);
				playerShip->decelerate(toggle);
				break;
			case SDLK_LEFT:
			case SDLK_a:
				playerShip->turn(toggle, false);
				//playerShip->addVelocity(0.1, 0);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				playerShip->turn(toggle, true);
				//playerShip->addVelocity(0.1, 0);
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

namespace AsteroidManager {
	using AsteroidsGame::TextureName;
	using AsteroidsGame::textures;
	using AsteroidsGame::asteroids;

	int asteroidCount = 0;
	int maxAsteroids = 10;

	int asteroidSpawnDelayMin = 2; // seconds
	int asteroidSpawnDelayMax = 8;

	int asteroidSizeMin = 32;
	int asteroidSizeMax = 128;

	SDL_TimerID timerID_asteroidSpawner = NULL;

	//

	Asteroid* createAsteroid();
	Uint32 asteroidSpawnerCallback(Uint32 interval, void* param);

	//

	Asteroid* createAsteroid() {
		// Get random texture
		TextureWrapper* texture = NULL;
		int randTexture = rand() % 3;
		switch (randTexture) {
		case 0:
			texture = textures[TextureName::ASTEROID1];
			break;
		case 1:
			texture = textures[TextureName::ASTEROID2];
			break;
		case 2:
			texture = textures[TextureName::ASTEROID3];
			break;
		}

		// Get random size
		int randSize = (rand() % asteroidSizeMax) + asteroidSizeMin;

		// TODO: Asteroid position
		// Get random position
		int posX = Asteroid::SCREEN_WIDTH / 2.0f;
		int posY = Asteroid::SCREEN_HEIGHT / 2.0f;

		// Create asteroid GameObject
		return new Asteroid(texture, randSize, randSize, posX, posY);
	}

	Uint32 asteroidSpawnerCallback(Uint32 interval, void* param) {
		Asteroid* asteroid = createAsteroid();
		asteroids.push_back(asteroid);

		// Call self with delay
		Uint32 nextSpawn = (rand() % asteroidSpawnDelayMax) + asteroidSpawnDelayMin;
		timerID_asteroidSpawner =
			SDL_AddTimer(nextSpawn * 1000, asteroidSpawnerCallback, NULL);

		return 0;
	}
}

int main(int argc, char* args[]) {
	using namespace AsteroidsGame;
	using namespace AsteroidManager;

	if (init() && loadTextures()) {
		GameObject::SCREEN_HEIGHT = SCREEN_HEIGHT;
		GameObject::SCREEN_WIDTH = SCREEN_WIDTH;

		createShip();
		timerID_asteroidSpawner =
			SDL_AddTimer(asteroidSpawnDelayMin * 1000, asteroidSpawnerCallback, NULL);

		SDL_Event e;

		bool quit = false;
		while (!quit) {
			// handle events
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

				tick(timeDelta);

				timeLastTick = timeTickNow;
				timeNextTick = timeTickNow + tickDelay;
			}

			SDL_SetRenderDrawColor(TextureWrapper::renderer, 0x00, 0x00, 0x00, 0xff);
			SDL_RenderClear(TextureWrapper::renderer);

			render();

			SDL_RenderPresent(TextureWrapper::renderer);
		} // while gameloop

		if (timerID_asteroidSpawner != NULL) {
			SDL_RemoveTimer(timerID_asteroidSpawner);
		}
	} // if init success

	close();

	return 0;
}
