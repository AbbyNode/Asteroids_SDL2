#pragma once

#include "gameobject.h"

class Asteroid : public GameObject {
public:
	Asteroid(TextureWrapper* textureWrapper, int width, int height, int posX, int posY);
};
