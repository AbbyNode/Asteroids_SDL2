#include "asteroid.h"

#include "util.h"

float Asteroid::speedMin = 0.05f;
float Asteroid::speedMax = 0.2f;

float Asteroid::spinMin = 0.1f;
float Asteroid::spinMax = 0.5f;

Asteroid::Asteroid(TextureWrapper* textureWrapper, int width, int height)
	: GameObject(textureWrapper, width, height, 0.4f) {
	using util::randomInt;
	using util::randomFloat;

	float posX = 0.0f;
	float posY = 0.0f;

	// Randomize which side of screen to spawn in
	int side = randomInt(0, 3);
	switch (side) {
	case 0: // top
		posX = randomFloat(0, SCREEN_WIDTH);
		posY = -height;
		break;
	case 1: // bottom
		posX = randomFloat(0, SCREEN_WIDTH);
		posY = SCREEN_HEIGHT + height;
		break;
	case 2: // left
		posX = -width;
		posY = randomFloat(0, SCREEN_HEIGHT);
		break;
	case 3: // right
		posX = SCREEN_WIDTH + width;
		posY = randomFloat(0, SCREEN_HEIGHT);
		break;
	}

	setPosition(posX, posY);

	float directionX = randomInt(0, 1);
	float directionY = randomInt(0, 1);
	float speedX = randomFloat(speedMin, speedMax) * (directionX == 0 ? 1 : -1);
	float speedY = randomFloat(speedMin, speedMax) * (directionY == 0 ? 1 : -1);
	addVelocity(speedX, speedY);

	float spin = randomFloat(spinMin, spinMax);
	setAngularMomentum(spin);
}
