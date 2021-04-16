#include "asteroid.h"

Asteroid::Asteroid(TextureWrapper* textureWrapper, int width, int height, int posX, int posY)
	: GameObject(textureWrapper, width, height) {
	setPosition(posX, posY);
}
