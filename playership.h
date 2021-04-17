#pragma once

#include "gameobject.h"

#include "texturewrapper.h"

class PlayerShip : public GameObject {
public:
	PlayerShip(TextureWrapper* textureWrapper);

	void tick(float delta);

	void accelerate(bool toggle);
	void decelerate(bool toggle);
	void turn(bool toggle, bool clockwise);

private:
	bool accelerating = false;
	bool decelerating = false;

	float turnSpeed = 0;
	float accelerationPerSecond = 0;
};
