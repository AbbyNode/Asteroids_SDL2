#pragma once

#include "gameobject.h"

#include "texturewrapper.h"

class Bullet : public GameObject {
public:
	Bullet(TextureWrapper* textureWrapper);

	void collisionCallback(GameObject* gameObject);
};
