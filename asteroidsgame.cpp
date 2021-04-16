#include <chrono>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "asteroid.h"
#include "gameobject.h"
#include "playership.h"
#include "texturewrapper.h"
#include "util.h"

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

	SDL_Window* window = nullptr;

	int ticksPerSecond = 60;
	uint32_t tickDelay = 1000 / ticksPerSecond; // delay in millis between ticks
	uint32_t timeLastTick = 0;
	uint32_t timeNextTick = 0;

	enum class TextureName {
		SHIP, BULLET, ASTEROID1, ASTEROID2, ASTEROID3
	};
	unordered_map<TextureName, TextureWrapper*> textures;

	PlayerShip* playerShip = nullptr;

	vector<GameObject*> asteroids;

	vector<GameObject*> bullets;

#pragma endregion variables

#pragma region forward declare

	bool init();
	void close();

	SDL_Texture* loadTextureFromFile(string path);
	bool loadTextures();
	bool createPlayerShip();
	bool shootBullet();

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
		if (window == nullptr) {
			err("Window Create");
			return false;
		}

		TextureWrapper::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (TextureWrapper::renderer == nullptr) {
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
		TextureWrapper::renderer = nullptr;

		SDL_DestroyWindow(window);
		window = nullptr;

		IMG_Quit();
		SDL_Quit();
	}

	//

	SDL_Texture* loadTextureFromFile(string path) {
		SDL_Texture* texture = nullptr;

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == nullptr) {
			err("Img load");
		}
		else {
			texture = SDL_CreateTextureFromSurface(TextureWrapper::renderer, loadedSurface);
			if (texture == nullptr) {
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
			if (texture == nullptr) {
				return false;
			}

			// Store texture into hashmap
			textures[name] = new TextureWrapper(texture, nullptr);
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

	bool createPlayerShip() {
		playerShip = new PlayerShip(textures[TextureName::SHIP]);
		playerShip->setCollidableGameObjects(&asteroids);
		return (playerShip != nullptr);
	}

	bool shootBullet() {
		return true;
	}

	//

	void tick(float delta) {
		playerShip->tick(delta);

		for (GameObject* asteroid : asteroids) {
			asteroid->tick(delta);
		}
	}

	void render() {
		for (GameObject* asteroid : asteroids) {
			asteroid->render();
		}

		playerShip->render();
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

namespace AsteroidSpawner {
	int maxCount = 4;

	int delayMin = 2; // seconds
	int delayMax = 8;

	int sizeMin = 32;
	int sizeMax = 128;

	SDL_TimerID timerID_asteroidSpawner = NULL;

	//

	Asteroid* createAsteroid();
	uint32_t asteroidSpawnerCallback(uint32_t interval, void* param);

	//

	Asteroid* createAsteroid() {
		using AsteroidsGame::TextureName;
		using AsteroidsGame::textures;

		// Get random texture
		TextureWrapper* texture = nullptr;
		int randTexture = util::randomInt(0, 2);
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
		int randSize = util::randomInt(sizeMin, sizeMax);
		
		// Create asteroid GameObject
		return new Asteroid(texture, randSize, randSize);
	}

	uint32_t asteroidSpawnerCallback(uint32_t interval, void* param) {
		using AsteroidsGame::asteroids;

		if (asteroids.size() < maxCount) {
			Asteroid* asteroid = createAsteroid();
			asteroids.push_back(asteroid);
		}

		// Call self with delay
		uint32_t nextSpawn = util::randomInt(delayMin, delayMax);
		timerID_asteroidSpawner =
			SDL_AddTimer(nextSpawn * 1000, asteroidSpawnerCallback, nullptr);

		return 0;
	}
}

int main(int argc, char* args[]) {
	using namespace AsteroidsGame;

	// Init and load
	if (init() && loadTextures()) {
		// Setup GameObject statics
		GameObject::SCREEN_HEIGHT = SCREEN_HEIGHT;
		GameObject::SCREEN_WIDTH = SCREEN_WIDTH;

		// Init random generator
		util::initRandom(std::chrono::system_clock::now().time_since_epoch().count());

		// Spawn player ship
		createPlayerShip();

		// Start spawning asteroids with timer
		{
			using namespace AsteroidSpawner;
			timerID_asteroidSpawner = SDL_AddTimer(delayMin * 1000, asteroidSpawnerCallback, nullptr);
		}

		// Store events to handle
		SDL_Event e;

		// Main game loop
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

			// Tick (every tickDelay)
			uint32_t timeTickNow = SDL_GetTicks();
			if (timeTickNow >= timeNextTick) {
				uint32_t timeDelta = timeTickNow - timeLastTick;

				tick(timeDelta);

				timeLastTick = timeTickNow;
				timeNextTick = timeTickNow + tickDelay;
			}

			// Render (every frame)
			SDL_SetRenderDrawColor(TextureWrapper::renderer, 0x00, 0x00, 0x00, 0xff);
			SDL_RenderClear(TextureWrapper::renderer);
			render();
			SDL_RenderPresent(TextureWrapper::renderer);
		} // while gameloop


		// Remove timer if exists
		if (AsteroidSpawner::timerID_asteroidSpawner != NULL) {
			SDL_RemoveTimer(AsteroidSpawner::timerID_asteroidSpawner);
		}
	} // if init success
	
	// Cleanup
	close();

	return 0;
}
