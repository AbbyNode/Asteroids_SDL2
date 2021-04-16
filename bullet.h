#pragma once

#include "gameobject.h"

#include "texturewrapper.h"

class Bullet : public GameObject {
public:
	Bullet(TextureWrapper* textureWrapper, float posX, float posY, float velX, float velY);

	void collisionCallback(GameObject* gameObject);
};
