#pragma once

#include <functional>

#include "gameobject.h"
#include "texturewrapper.h"

class Bullet : public GameObject {
public:
	Bullet(TextureWrapper* textureWrapper, float posX, float posY, float velX, float velY
		, std::function<void(GameObject* thisBullet)> destroyCallback);

	void tick(float delta);

private:
	std::function<void(GameObject* thisBullet)> destroyCallback;
};
