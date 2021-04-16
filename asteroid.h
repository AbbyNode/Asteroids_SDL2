#pragma once

#include "gameobject.h"

class Asteroid : public GameObject {
public:
	static float speedMin;
	static float speedMax;

	static float spinMin;
	static float spinMax;

	Asteroid(TextureWrapper* textureWrapper, int width, int height);
};
