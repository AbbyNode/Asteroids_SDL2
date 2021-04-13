#pragma once

#include "texturewrapper.h"

class GameObject {
public:
	GameObject(TextureWrapper* textureWrapper, int width, int height);
	~GameObject();

	// 
	void tick(float delta);

	// Display game object to renderer
	void render();

	void addVelocity(float x, float y);
	void reduceVelocity(float x, float y);

	float getSpeed();

private:
	float posX = 0, posY = 0;
	float rotation = 0;
	int width, height;

	float velX = 0, velY = 0;
	float maxSpeed = 10000;

	TextureWrapper* textureWrapper;
};
