#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const char* TITLE = "Asteroids";

enum class KeyPressSurfaces {
	SHIP,
	ASTEROID1,
	ASTEROID2,
	ASTEROID3
};

bool init();
bool loadMedia();
void close();
SDL_Surface* loadSurface(string path);

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

const int gImageCount = 4;
SDL_Surface* gKeyPressImages[gImageCount];
SDL_Surface* gCurrentImage = NULL;

void err(string name) {
	printf("Error | %s |\n%s", name.c_str(), SDL_GetError());
}

void err_img(string name) {
	printf("Error | %s |\n%s", name.c_str(), IMG_GetError());
}

//

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		err("SDL Init");
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			err("Window Create");
		}
		else {
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				err_img("SDL Img Init");
			}
			else {
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}

	return success;
}

//

int getImgIndex(KeyPressSurfaces img) {
	int index = 0;

	switch (img) {
	case KeyPressSurfaces::SHIP:
		index = 0;
		break;
	case KeyPressSurfaces::ASTEROID1:
		index = 1;
		break;
	case KeyPressSurfaces::ASTEROID2:
		index = 2;
		break;
	case KeyPressSurfaces::ASTEROID3:
		index = 3;
		break;
	}

	return index;
}

bool loadMediaHelper(KeyPressSurfaces img, string path, string name) {
	bool success = true;

	int index = getImgIndex(img);

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		err(name);
		success = false;
	}
	else {
		SDL_Surface* optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL) {
			err(name);
			success = false;
		}
		else {
			gKeyPressImages[index] = optimizedSurface;
		}

		SDL_FreeSurface(loadedSurface);
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	if (!loadMediaHelper(KeyPressSurfaces::SHIP, "resources/ship.png", "ship")) {
		success = false;
	}

	if (!loadMediaHelper(KeyPressSurfaces::ASTEROID1, "resources/asteroid1.png", "asteroid1")) {
		success = false;
	}

	if (!loadMediaHelper(KeyPressSurfaces::ASTEROID2, "resources/asteroid2.png", "asteroid2")) {
		success = false;
	}

	if (!loadMediaHelper(KeyPressSurfaces::ASTEROID3, "resources/asteroid3.png", "asteroid3")) {
		success = false;
	}

	return success;
}

void close() {
	for (int i = 0; i < gImageCount; i++) {
		SDL_FreeSurface(gKeyPressImages[i]);
		gKeyPressImages[i] = NULL;
	}

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

SDL_Surface* loadSurface(string path) {
	//SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

	if (loadedSurface == NULL) {
		string msg = "Image Load " + path;
		err(msg.c_str());
	}

	return loadedSurface;
}

int main(int argc, char* args[]) {
	if (!init()) {
		printf("Failed to init");
	}
	else {
		if (!loadMedia()) {
			printf("Failed to load media");
		}
		else {
			bool quit = false;

			SDL_Event e;

			gCurrentImage = gKeyPressImages[0];

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
						case SDLK_UP:
							gCurrentImage = gKeyPressImages[getImgIndex(KeyPressSurfaces::SHIP)];
							break;
						case SDLK_LEFT:
							gCurrentImage = gKeyPressImages[getImgIndex(KeyPressSurfaces::ASTEROID1)];
							break;
						case SDLK_DOWN:
							gCurrentImage = gKeyPressImages[getImgIndex(KeyPressSurfaces::ASTEROID2)];
							break;
						case SDLK_RIGHT:
							gCurrentImage = gKeyPressImages[getImgIndex(KeyPressSurfaces::ASTEROID3)];
							break;
						}
					}
				}

				SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;

				SDL_BlitScaled(gCurrentImage, NULL, gScreenSurface, &stretchRect);
				//SDL_BlitSurface(gCurrentImage, NULL, gScreenSurface, NULL);

				SDL_UpdateWindowSurface(gWindow);
				//SDL_Delay(2000);
			}
		}
	}

	close();

	return 0;
}
