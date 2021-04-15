#pragma once

#include "gameobject.h"
#include "texturewrapper.h"

class PlayerShip : public GameObject {
public:
	PlayerShip(TextureWrapper* textureWrapper);

	virtual void tick(float delta);

	void accelerate(bool toggle, bool forward);
	void turn(bool toggle, bool clockwise);

private:
	float accelerationX = 0, accelerationY = 0;

	float turnSpeed = 0.2;
	float accelerationPerSecond = 0.001;
};
