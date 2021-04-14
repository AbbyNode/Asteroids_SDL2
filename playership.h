#pragma once

#include "gameobject.h"

class PlayerShip : public GameObject {
public:
	PlayerShip(TextureWrapper* textureWrapper) : GameObject(textureWrapper, 32, 32) {}

	void startAccelerate();
	void stopAccelerate();
	void startDecelerate();
	void stopDecelerate();
};
