#pragma once

#include "gameobject.h"

class PlayerShip : public GameObject {
	PlayerShip(TextureWrapper* textureWrapper) : GameObject(textureWrapper, 32, 32);
};
