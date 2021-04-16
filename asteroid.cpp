#include "asteroid.h"

#include "util.h"

float Asteroid::spinMin = 0.1;
float Asteroid::spinMax = 0.5;

Asteroid::Asteroid(TextureWrapper* textureWrapper, int width, int height)
	: GameObject(textureWrapper, width, height) {
	using util::randomInt;
	using util::randomFloat;

	// Randomize which side of screen to spawn in
	int side = randomInt(0, 3); // TODO random
	switch (side) {
	case 0: // top

		break;
	case 1: // bottom

		break;
	case 2: // left

		break;
	case 3: // right

		break;
	}

	// TODO: Asteroid position
	// Get random position
	int posX = Asteroid::SCREEN_WIDTH / 2.0f;
	int posY = Asteroid::SCREEN_HEIGHT / 2.0f;
	setPosition(posX, posY);

	float spin = randomFloat(spinMin, spinMax);
	setAngularMomentum(1);
}
